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
	if (function_name == "TopK") {
	  auto &need_node = postprocess_function_config.begin()->second["need"];
	  if (!need_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(TopK) must have a need(bool) node.")
		return false;
	  }
	  need_topk_ = need_node.as<bool>();

	  auto &k_node = postprocess_function_config.begin()->second["k"];
	  if (!k_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(TopK) must have a k(size_t) node.")
		return false;
	  }
	  k_ = k_node.as<size_t>();

	  auto &min_confidence_node = postprocess_function_config.begin()->second["min_confidence"];
	  if (!min_confidence_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(TopK) must have a min_confidence(float) node.")
		return false;
	  }
	  min_confidence_ = min_confidence_node.as<float>();
	} else if (function_name == "Softmax") {
	  auto &need_node = postprocess_function_config.begin()->second["need"];
	  if (!need_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(TopK) must have a need(bool) node.")
		return false;
	  }
	  need_softmax_ = need_node.as<bool>();
	} else {
	  BREEZE_DEPLOY_LOGGER_ERROR("The postprocess name only supports [TopK,Softmax], "
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

  BreezeDeployMat breeze_deploy_mat(input_mat);
  for (const auto &preprocess_function : preprocess_functions_) {
	if (!preprocess_function->Run(breeze_deploy_mat)) {
	  BREEZE_DEPLOY_LOGGER_ERROR("Failed to run preprocess.")
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
bool ClassificationModel::Postprocess() {
  // 判断是否需要进行Softmax
  if (need_softmax_) {

  }
  return true;
}
bool ClassificationModel::Predict(const cv::Mat &input_mat, ClassificationResult &classification_result) {
  auto result_predict = BreezeDeployModel::Predict(input_mat);
  if (!result_predict) {
	return false;
  }

  // 清空结果
  classification_result.Clear();

  // 先判断是否需要进行TopK
  if (need_topk_) {
	auto result_topk = utils::data_process::TopK<float>(output_tensor_vector_[0], classification_result, k_, min_confidence_);
	if (!result_topk) {
	  return false;
	}
  }
  return true;
}
#if 0
bool ClassificationModel::Predict(const cv::Mat &input_mat, ClassificationFeatureResult &label_result) {
  auto result_predict = BreezeDeployModel::Predict(input_mat);
  if (!result_predict) {
	return false;
  }

  auto &tensor = output_tensor_vector_[0];
  auto tensor_data_ptr = reinterpret_cast<float *>(tensor.GetTensorDataPointer());
  auto tensor_data_size = tensor.GetTensorSize();
  label_result.feature_vector = std::vector<float>(tensor_data_ptr, tensor_data_ptr + tensor_data_size);
  return true;
}
double ClassificationModel::CosineSimilarity(const ClassificationFeatureResult &a,
											 const ClassificationFeatureResult &b) {
  if (a.feature_vector.size() != b.feature_vector.size()) {
	BREEZE_DEPLOY_LOGGER_ERROR(
		"The size of Vector A and B must be equal. The size of vector A is {}, while the size of vector B is also {}.",
		a.feature_vector.size(),
		b.feature_vector.size())
	return 0;
  }

  if ((a.feature_vector.empty()) || (b.feature_vector.empty())) {
	BREEZE_DEPLOY_LOGGER_ERROR(
		"The size of vectors A and B must be greater than 0. The size of vector A is {}, while the size of vector B is also {}.",
		a.feature_vector.size(),
		b.feature_vector.size())
	return 0;
  }

  auto feature_vector_a = a.feature_vector;
  auto feature_vector_b = b.feature_vector;
  Eigen::Map<Eigen::VectorXf> eigen_vector_a(feature_vector_a.data(), static_cast<long>(feature_vector_a.size()));
  Eigen::Map<Eigen::VectorXf> eigen_vector_b(feature_vector_b.data(), static_cast<long>(feature_vector_b.size()));
  return eigen_vector_a.dot(eigen_vector_b) / (eigen_vector_a.norm() * eigen_vector_b.norm());
}
#endif
size_t ClassificationModel::GetFeatureVectorLength() {
  if (breeze_deploy_backend_ == nullptr) {
	BREEZE_DEPLOY_LOGGER_ERROR("This model uses a null pointer for the inference backend. "
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