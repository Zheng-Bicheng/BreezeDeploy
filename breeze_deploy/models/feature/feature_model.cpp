// Copyright (c) 2024/2/1 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/feature/feature_model.h"
#include "breeze_deploy/utils/data_process/normalize_l2/normalize_l2.h"

namespace breeze_deploy {
namespace models {
bool FeatureModel::InitializeBackend(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  auto result_init = BDModel::InitializeBackend(breeze_deploy_backend_option);
  if (!result_init) {
	BDLOGGER_ERROR("Failed to initialize the backend. "
							   "Please check if the backend configuration parameters are correct.");
	return false;
  }

  auto input_tensor_size = breeze_deploy_backend_->GetInputTensorSize();
  auto output_tensor_size = breeze_deploy_backend_->GetOutputTensorSize();
  if ((input_tensor_size != 1) || (output_tensor_size != 1)) {
	BDLOGGER_ERROR(
		"The classification model only supports input and output tensor with a size of 1. "
		"However, the input tensor is of size {}, and the output tensor is of size {}.",
		input_tensor_size,
		output_tensor_size)
	return false;
  }
  return true;
}
bool FeatureModel::ReadPostprocessYAML() {
  YAML::Node yaml_config;
  try {
	yaml_config = YAML::LoadFile(config_file_path_);
  } catch (YAML::BadFile &e) {
	BDLOGGER_ERROR("Failed to load yaml file: {}.", config_file_path_)
	return false;
  }

  // Get postprocess root node
  auto postprocess_config = yaml_config["postprocess"];
  // Traverse postprocess root node branches
  for (const auto &postprocess_function_config : postprocess_config) {
	auto function_name = postprocess_function_config.begin()->first.as<std::string>();
	if (function_name == "Softmax") {
	  auto &need_node = postprocess_function_config.begin()->second;
	  if (!need_node) {
		BDLOGGER_ERROR("The function(TopK) must have a value.")
		return false;
	  }
	  need_softmax_ = need_node.as<bool>();
	} else if (function_name == "NormalizeL2") {
	  auto &need_node = postprocess_function_config.begin()->second;
	  if (!need_node) {
		BDLOGGER_ERROR("The function(NormalizeL2) must have a value.")
		return false;
	  }
	  need_normalize_l2_ = need_node.as<bool>();
	} else {
	  BDLOGGER_ERROR("The postprocess name only supports [NormalizeL2, Softmax], "
								 "but now it is called {}.", function_name)
	  return false;
	}
  }
  return true;
}
bool FeatureModel::Preprocess(const cv::Mat &input_mat) {
  if (input_mat.empty()) {
	BDLOGGER_ERROR("input_mat is empty.")
	return false;
  }

  BreezeDeployMat breeze_deploy_mat(input_mat);
  for (const auto &preprocess_function : preprocess_functions_) {
	if (!preprocess_function->Run(breeze_deploy_mat)) {
	  BDLOGGER_ERROR("Failed to run preprocess.")
	  return false;
	}
  }

  auto tensor_data = breeze_deploy_mat.GetMat().data;
  auto tensor_data_type = breeze_deploy_mat.GetMatDataType();
  auto c = breeze_deploy_mat.GetChannel();
  auto h = breeze_deploy_mat.GetHeight();
  auto w = breeze_deploy_mat.GetWidth();
  if (breeze_deploy_mat.GetMatDataFormat() == BreezeDeployMatFormat::CHW) {
	input_tensor_vector_[0].SetTensorData(tensor_data, {1, c, h, w}, tensor_data_type);
  } else {
	input_tensor_vector_[0].SetTensorData(tensor_data, {1, h, w, c}, tensor_data_type);
  }
  return true;
}
bool FeatureModel::Postprocess() {
  // 判断是否需要进行Softmax
  if (need_softmax_) {

  }
  return true;
}
bool FeatureModel::Predict(const cv::Mat &input_mat, FeatureResult &label_result) {
  auto result_predict = BDModel::Predict(input_mat);
  if (!result_predict) {
	return false;
  }

  auto &tensor = output_tensor_vector_[0];
  auto tensor_data_ptr = reinterpret_cast<float *>(tensor.GetTensorDataPointer());
  auto tensor_data_size = tensor.GetTensorSize();
  label_result.feature_vector = std::vector<float>(tensor_data_ptr, tensor_data_ptr + tensor_data_size);
  if (need_normalize_l2_){
	utils::data_process::NormalizeL2(label_result.feature_vector);
  }

  return true;
}
size_t FeatureModel::GetFeatureLength() {
  if (breeze_deploy_backend_ == nullptr) {
	BDLOGGER_ERROR("This model uses a null pointer for the inference backend. "
							   "Please check if the model backend has been initialized.")
	return false;
  }
  auto tensor_info = breeze_deploy_backend_->GetOutputTensorInfo()[0];

  size_t tensor_length = 1;
  for (auto shape : tensor_info.tensor_shape) {
	tensor_length *= shape;
  }
  return tensor_length;
}
}
}