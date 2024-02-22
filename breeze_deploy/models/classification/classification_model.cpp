// Copyright (c) 2023/12/28 Zheng-Bicheng. All Rights Reserved.
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
#include <Eigen/Dense>
#include <fstream>
#include "breeze_deploy/models/classification/classification_model.h"
#include "breeze_deploy/utils/data_process/top_k/top_k.h"
#include "breeze_deploy/utils/data_process/softmax/softmax.h"

namespace breeze_deploy {
namespace models {
bool ClassificationModel::InitializeBackend(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  auto result_init = BreezeDeployModel::InitializeBackend(breeze_deploy_backend_option);
  if (!result_init) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to initialize the backend. "
							   "Please check if the backend configuration parameters are correct.");
	return false;
  }

  auto input_tensor_size = breeze_deploy_backend_->GetInputTensorSize();
  auto output_tensor_size = breeze_deploy_backend_->GetOutputTensorSize();
  if ((input_tensor_size != 1) || (output_tensor_size != 1)) {
	BREEZE_DEPLOY_LOGGER_ERROR(
		"The classification model only supports input and output tensor with a size of 1. "
		"However, the input tensor is of size {}, and the output tensor is of size {}.",
		input_tensor_size,
		output_tensor_size)
	return false;
  }
  return true;
}
bool ClassificationModel::ReadPostprocessYAML() {
  YAML::Node yaml_config;
  try {
	yaml_config = YAML::LoadFile(config_file_path_);
  } catch (YAML::BadFile &e) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to load yaml file: {}.", config_file_path_)
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
		BREEZE_DEPLOY_LOGGER_ERROR("The function(TopK) must have a need(bool) node.")
		return false;
	  }
	  need_softmax_ = need_node.as<bool>();
	} else {
	  BREEZE_DEPLOY_LOGGER_ERROR("The postprocess name only supports [Softmax], "
								 "but now it is called {}.", function_name)
	  return false;
	}
  }
  return true;
}
bool ClassificationModel::Preprocess(const cv::Mat &input_mat) {
  if (input_mat.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("input_mat is empty.")
	return false;
  }

  input_mat_ = BreezeDeployMat(input_mat);
  for (const auto &preprocess_function : preprocess_functions_) {
	if (!preprocess_function->Run(input_mat_)) {
	  BREEZE_DEPLOY_LOGGER_ERROR("Failed to run preprocess.")
	  return false;
	}
  }

  auto tensor_data = input_mat_.GetMat().data;
  auto tensor_data_type = input_mat_.GetMatDataType();
  auto c = input_mat_.GetChannel();
  auto h = input_mat_.GetHeight();
  auto w = input_mat_.GetWidth();
  if (input_mat_.GetMatDataFormat() == BreezeDeployMatFormat::CHW) {
	input_tensor_vector_[0].SetTensorData(tensor_data, {1, c, h, w}, tensor_data_type);
  } else {
	input_tensor_vector_[0].SetTensorData(tensor_data, {1, h, w, c}, tensor_data_type);
  }
  return true;
}
bool ClassificationModel::Postprocess() {
  // 判断是否需要进行Softmax
  if (need_softmax_) {
    utils::data_process::Softmax(output_tensor_vector_[0]);
  }
  return true;
}
bool ClassificationModel::Predict(const cv::Mat &input_mat,
								  ClassificationResult &classification_result,
								  size_t k,
								  float min_confidence) {
  auto result_predict = BreezeDeployModel::Predict(input_mat);
  if (!result_predict) {
	return false;
  }

  // 清空结果
  classification_result.Clear();

  // 先判断是否需要进行TopK
  auto result_topk =
	  utils::data_process::TopK<float>(output_tensor_vector_[0], classification_result, k, min_confidence);
  if (!result_topk) {
	return false;
  }
  return true;
}
}
}