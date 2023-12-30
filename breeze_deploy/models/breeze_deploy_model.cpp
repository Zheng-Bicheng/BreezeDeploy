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

#include "breeze_deploy/backends/onnx_backend/onnx_backend.h"
#include "breeze_deploy/core/breeze_deploy_logger.h"
#include "breeze_deploy/models/breeze_deploy_model.h"

namespace breeze_deploy {
namespace models {
BreezeDeployModel::BreezeDeployModel(const std::string &model_path, const std::string &config_file_path) {
  model_path_ = model_path;
  config_file_path_ = config_file_path;
}
bool BreezeDeployModel::ReadPreprocessYAML() {
  preprocess_function_vector_.clear();
  YAML::Node yaml_config;
  try {
	yaml_config = YAML::LoadFile(config_file_path_);
  } catch (YAML::BadFile &e) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to load yaml file: {}.", config_file_path_)
	return false;
  }

  // Get preprocess root node
  auto preprocess_config = yaml_config["preprocess"];
  if (!preprocess_config) {
	BREEZE_DEPLOY_LOGGER_ERROR("The yaml config must have a preprocess element.")
	return false;
  }

  // Traverse preprocess root node branches
  for (const auto &preprocess_function_config : preprocess_config) {
	auto function_name = preprocess_function_config.begin()->first.as<std::string>();
	if (function_name == "Resize") {
	  auto &resize_config_node = preprocess_function_config.begin()->second["size"];
	  if (!resize_config_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(Resize) must have a size element.")
		return false;
	  }
	  auto target_size = resize_config_node.as<std::vector<int>>();
	  preprocess_function_vector_.push_back(std::make_shared<Resize>(target_size[0], target_size[1]));
	} else if (function_name == "BGRToRGB") {
	  preprocess_function_vector_.push_back(std::make_shared<BGRToRGB>());
	} else if (function_name == "Normalize") {
	  auto &mean_config_node = preprocess_function_config.begin()->second["mean"];
	  auto &std_config_node = preprocess_function_config.begin()->second["std"];
	  if (!mean_config_node || !std_config_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(Normalize) must have mean and std element.")
		return false;
	  }
	  auto mean = mean_config_node.as<std::vector<float>>();
	  auto std = std_config_node.as<std::vector<float>>();
	  preprocess_function_vector_.push_back(std::make_shared<Normalize>(mean, std));
	} else if (function_name == "HWCToCHW") {
	  preprocess_function_vector_.push_back(std::make_shared<HWCToCHW>());
	} else {
	  BREEZE_DEPLOY_LOGGER_ERROR("The preprocess function name only supports [Resize, BGRToRGB, Normalize, HWCToCHW], "
								 "but now it is called {}.", function_name)
	}
  }
  return true;
}
bool BreezeDeployModel::Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  // Read preprocess config yaml
  if (!ReadPreprocessYAML()) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to read preprocess function from yaml: {}.", config_file_path_);
	return false;
  }

  // Copy backend option and set model_path.
  breeze_deploy_backend_option_ = breeze_deploy_backend_option;
  breeze_deploy_backend_option_.SetModelPath(model_path_);
  breeze_deploy_backend_ = std::make_shared<ONNXBackend>();
  auto result = breeze_deploy_backend_->Initialize(breeze_deploy_backend_option_);
  return result;
}
bool BreezeDeployModel::Preprocess(const cv::Mat &input_mat) {
  BREEZE_DEPLOY_MODEL_LOGGER_UN_SUPPORT_API
  return false;
}
bool BreezeDeployModel::Infer() {
  BREEZE_DEPLOY_MODEL_LOGGER_UN_SUPPORT_API
  return false;
}
bool BreezeDeployModel::Postprocess() {
  BREEZE_DEPLOY_MODEL_LOGGER_UN_SUPPORT_API
  return false;
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
