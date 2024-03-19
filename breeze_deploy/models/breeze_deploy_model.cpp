// Copyright (c) 2023/12/26 Zheng-Bicheng. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <yaml-cpp/yaml.h>

#include "breeze_deploy/backends/rknpu_backend/rknpu_backend.h"
#include "breeze_deploy/backends/onnx_backend/onnx_backend.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"
#include "breeze_deploy/models/breeze_deploy_model.h"

namespace breeze_deploy {
namespace models {
BreezeDeployModel::BreezeDeployModel(const std::string &model_path, const std::string &config_file_path) {
  model_path_ = model_path;
  config_file_path_ = config_file_path;
}
bool BreezeDeployModel::ReadPreprocessYAML() {
  preprocess_functions_.clear();
  YAML::Node yaml_config;
  try {
    yaml_config = YAML::LoadFile(config_file_path_);
  } catch (YAML::BadFile &e) {
    BDLOGGER_ERROR("Failed to load yaml file: {}.", config_file_path_)
    return false;
  }

  // Get preprocess root node
  auto preprocess_config = yaml_config["preprocess"];
  if (!preprocess_config) {
    BDLOGGER_ERROR("The yaml config must have a preprocess node.")
    return false;
  }

  // Traverse preprocess root node branches
  for (const auto &preprocess_function_config : preprocess_config) {
    auto function_name = preprocess_function_config.begin()->first.as<std::string>();
    if (function_name == "Resize") {
      auto &resize_width_node = preprocess_function_config.begin()->second["width"];
      if (!resize_width_node) {
        BDLOGGER_ERROR("The function(Resize) must have a width(int) node.")
        return false;
      }
      auto target_width_size = resize_width_node.as<int>();

      auto &resize_height_node = preprocess_function_config.begin()->second["height"];
      if (!resize_height_node) {
        BDLOGGER_ERROR("The function(Resize) must have a height(int) node.")
        return false;
      }
      auto target_height_size = resize_height_node.as<int>();
      preprocess_functions_.push_back(std::make_shared<Resize>(target_width_size, target_height_size));
    } else if (function_name == "BGRToRGB") {
      preprocess_functions_.push_back(std::make_shared<BGRToRGB>());
    } else if (function_name == "Normalize") {
      if (breeze_deploy_backend_->BackendName() == "RKNPUBackend"){
        continue;
      }
      auto &mean_node = preprocess_function_config.begin()->second["mean"];
      auto &std_node = preprocess_function_config.begin()->second["std"];
      if (!mean_node || !std_node) {
        BDLOGGER_ERROR("The function(Normalize) must have mean(float) and std(float) node.")
        return false;
      }
      auto mean = mean_node.as<std::vector<float>>();
      auto std = std_node.as<std::vector<float>>();
      preprocess_functions_.push_back(std::make_shared<Normalize>(mean, std));
    } else if (function_name == "HWCToCHW") {
      if (breeze_deploy_backend_->BackendName() == "RKNPUBackend"){
        continue;
      }
      preprocess_functions_.push_back(std::make_shared<HWCToCHW>());
    } else if (function_name == "LetterBox") {
      // Get LetterBox width
      auto &resize_width_node = preprocess_function_config.begin()->second["width"];
      if (!resize_width_node) {
        BDLOGGER_ERROR("The function(LetterBox) must have a width(int) node.")
        return false;
      }
      auto target_width_size = resize_width_node.as<int>();

      // Get LetterBox height
      auto &resize_height_node = preprocess_function_config.begin()->second["height"];
      if (!resize_height_node) {
        BDLOGGER_ERROR("The function(LetterBox) must have a height(int) node.")
        return false;
      }
      auto target_height_size = resize_height_node.as<int>();

      // Get LetterBox scalar
      auto &pad_scalar_node = preprocess_function_config.begin()->second["scalar"];
      if (!pad_scalar_node) {
        BDLOGGER_ERROR("The function(LetterBox) must have a scalar(std::array<float, 3>) node.")
        return false;
      }
      auto target_scalar_size = pad_scalar_node.as<std::array<float, 3>>();
      preprocess_functions_.push_back(std::make_shared<LetterBox>(target_width_size,
                                                                  target_height_size,
                                                                  target_scalar_size));
    } else {
      BDLOGGER_ERROR(
          "The preprocess function name only supports [Resize, BGRToRGB, NormalizeL2, HWCToCHW, LetterBox], "
          "but now it is called {}.",
          function_name)
      return false;
    }
  }
  return true;
}
bool BreezeDeployModel::InitializeBackend(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  breeze_deploy_backend_option_ = breeze_deploy_backend_option;
  breeze_deploy_backend_option_.SetModelPath(model_path_);

  if (model_path_.find(".onnx") != std::string::npos) {
    breeze_deploy_backend_ = std::make_unique<ONNXBackend>();
  }

#ifdef ENABLE_RKNN_RUNTIME
  if (model_path_.find(".rknn") != std::string::npos) {
    breeze_deploy_backend_ = std::make_unique<RKNPUBackend>();
  }
#endif

  if (breeze_deploy_backend_ == nullptr) {
    BDLOGGER_ERROR(
        "The model file path is incorrect and the corresponding inference backend was not recognized.")
    return false;
  }

  auto result_init = breeze_deploy_backend_->Initialize(breeze_deploy_backend_option_);
  if (!result_init) {
    BDLOGGER_ERROR("Failed to initialize backend.")
    return false;
  }
  input_tensor_vector_.resize(breeze_deploy_backend_->GetInputTensorSize());
  output_tensor_vector_.resize(breeze_deploy_backend_->GetOutputTensorSize());

  for (auto &tensor_info : breeze_deploy_backend_->GetOutputTensorInfo()) {
    auto input_batch = tensor_info.tensor_shape[0];
    if (input_batch != 1 && input_batch != -1) {
      BDLOGGER_ERROR(
          "The current classification model only supports input and output vectors with a batch of 1. However, the batch is {}.",
          input_batch)
      return false;
    }
  }
  return true;
}
bool BreezeDeployModel::Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  if (!InitializeBackend(breeze_deploy_backend_option)) {
    BDLOGGER_ERROR("Failed to initialize backend.")
    return false;
  }

  if (!ReadPreprocessYAML()) {
    BDLOGGER_ERROR("Failed to read preprocess function from yaml: {}.", config_file_path_)
    return false;
  }

  if (!ReadPostprocessYAML()) {
    BDLOGGER_ERROR("Failed to read postprocess function from yaml: {}.", config_file_path_)
    return false;
  }
  return true;
}
bool BreezeDeployModel::Infer() {
  if (breeze_deploy_backend_ == nullptr) {
    BDLOGGER_ERROR("This model uses a null pointer for the inference backend. "
                               "Please check if the model backend has been initialized.")
    return false;
  }
  return breeze_deploy_backend_->Infer(input_tensor_vector_, output_tensor_vector_);
}
bool BreezeDeployModel::Predict(const cv::Mat &input_mat) {
  if (!Preprocess(input_mat)) {
    return false;
  }
  if (!Infer()) {
    return false;
  }
  if (!Postprocess()) {
    return false;
  }
  return true;
}
}
}
