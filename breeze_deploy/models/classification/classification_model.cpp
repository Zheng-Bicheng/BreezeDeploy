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
#include <fstream>
#include "breeze_deploy/models/classification/classification_model.h"
namespace breeze_deploy {
namespace models {
ClassificationModel::ClassificationModel(const std::string &model_path, const std::string &config_file_path)
	: BreezeDeployModel(model_path, config_file_path) {
  input_tensor_vector_.resize(1);
  output_tensor_vector_.resize(1);
}
bool ClassificationModel::Preprocess(const cv::Mat &input_mat) {
  if (input_mat.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("input_mat is empty.")
	return false;
  }

  BreezeDeployMat breeze_deploy_mat(input_mat);
  for (const auto &preprocess_function : preprocess_functions_) {
	if (!preprocess_function->Run(breeze_deploy_mat)) {
	  BREEZE_DEPLOY_LOGGER_ERROR("Failed to run preprocess_function.")
	  return false;
	}
  }

  auto tensor_data = breeze_deploy_mat.GetMat().data;
  auto tensor_data_type = breeze_deploy_mat.GetMatDataType();
  auto c = breeze_deploy_mat.GetChannel();
  auto h = breeze_deploy_mat.GetHeight();
  auto w = breeze_deploy_mat.GetWidth();
  if (breeze_deploy_mat.GetMatDataFormat() == BreezeDeployDataFormat::CHW) {
	input_tensor_vector_[0].SetTensorData(tensor_data, {1, c, h, w}, tensor_data_type);
  } else {
	input_tensor_vector_[0].SetTensorData(tensor_data, {1, h, w, c}, tensor_data_type);
  }
  return true;
}
bool ClassificationModel::Infer() {
  return breeze_deploy_backend_->Infer(input_tensor_vector_, output_tensor_vector_);
}
bool ClassificationModel::ReadPostprocessYAML() {
  postprocess_function_vector_.clear();
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
	  auto &k_config_node = postprocess_function_config.begin()->second["k"];
	  if (!k_config_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(TopK) must have a k element.")
		return false;
	  }
	  auto k = k_config_node.as<size_t>();
	  postprocess_function_vector_.push_back(std::make_shared<TopK>(k));
	} else {
	  BREEZE_DEPLOY_LOGGER_ERROR("The postprocess name only supports [TopK], "
								 "but now it is called {}.", function_name)
	  return false;
	}
  }
  return true;
}
bool ClassificationModel::Postprocess() {
  classification_results_.clear();
  for (const auto &i : postprocess_function_vector_) {
	i->Run(output_tensor_vector_[0], classification_results_);
  }

  if (classification_labels_.empty()) {
	BREEZE_DEPLOY_LOGGER_WARN("The classification labels is empty.");
	return true;
  }

  for (auto &classification_result : classification_results_) {
	classification_result.label = classification_labels_[classification_result.index];
  }
  return true;
}
bool ClassificationModel::SetLabel(const std::string &label_file_path) {
  classification_labels_.clear();
  std::ifstream input_file(label_file_path);
  if (!input_file.is_open()) {
	BREEZE_DEPLOY_LOGGER_ERROR("Could not open file: {}.", label_file_path)
	return false;
  }
  std::string line;
  while (std::getline(input_file, line)) {
	classification_labels_.push_back(line);
  }
  input_file.close();
  return true;
}
const std::vector<ClassificationResult> &ClassificationModel::GetClassificationResults() {
  return classification_results_;
}
}
}