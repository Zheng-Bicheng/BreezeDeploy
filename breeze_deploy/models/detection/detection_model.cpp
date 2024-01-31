// Copyright (c) 2024/1/22 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/detection/detection_model.h"

namespace breeze_deploy {
namespace models {
bool DetectionModel::Preprocess(const cv::Mat &input_mat) {
  if (input_mat.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("input_mat is empty.")
	return false;
  }

  // Do preprocess
  BreezeDeployMat breeze_deploy_mat(input_mat);
  for (const auto &preprocess_function : preprocess_functions_) {
	if (!preprocess_function->Run(breeze_deploy_mat)) {
	  BREEZE_DEPLOY_LOGGER_ERROR("Failed to run preprocess.")
	  return false;
	}
  }

  // Set data to tensor
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

  // Get resize radio and pad height/width.
  for (const auto &preprocess_function : preprocess_functions_) {
	if (preprocess_function->FunctionName() != "LetterBox") {
	  continue;
	}
	pad_height_ = preprocess_function->GetPadHeight();
	pad_width_ = preprocess_function->GetPadWidth();
	radio_ = preprocess_function->GetRadio();
  }
  return true;
}
bool DetectionModel::ReadPostprocessYAML() {
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
	if (function_name == "landmark_num") {
	  auto &landmark_num_node = postprocess_function_config.begin()->second;
	  if (!landmark_num_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(landmark_num) must have a landmark_num element.")
		return false;
	  }
	  landmark_num_ = landmark_num_node.as<int>();
	} else {
	  BREEZE_DEPLOY_LOGGER_ERROR("The postprocess name only supports [landmark_num], "
								 "but now it is called {}.", function_name)
	  return false;
	}
  }
  return true;
}
bool DetectionModel::Postprocess() {
  return true;
}
cv::Mat DetectionModel::Draw(const cv::Mat &mat, const DetectionResult &detection_result) {
  if (detection_result.GetSize() == 0) {
	return {};
  }

  for (int i = 0; i < detection_result.GetSize(); ++i) {
	auto rect_vector = detection_result.rect_vector[i];
	cv::rectangle(mat, rect_vector, cv::Scalar(0, 0, 255), 1);
  }

  if (detection_result.landmarks_vector.empty()) {
	return mat;
  }

  for (int i = 0; i < detection_result.GetSize(); ++i) {
	auto landmarks_vector = detection_result.landmarks_vector[i];
	for (const auto &landmark : landmarks_vector) {
	  cv::circle(mat, landmark, 1, cv::Scalar(0, 0, 255), 1);
	}
  }
  return mat;
}
}
}