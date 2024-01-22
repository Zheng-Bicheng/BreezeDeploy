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

#include "breeze_deploy/models/detection/yolov5/yolov_5.h"
namespace breeze_deploy {
namespace models {
YOLOV5::YOLOV5(const std::string &model_path, const std::string &config_file_path)
	: DetectionModel(model_path, config_file_path) {
  input_tensor_vector_.resize(1);
  output_tensor_vector_.resize(1);
}
bool YOLOV5::Preprocess(const cv::Mat &input_mat) {
  if (input_mat.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("input_mat is empty.")
	return false;
  }

  BreezeDeployMat breeze_deploy_mat(input_mat);
  for (const auto &preprocess_function : preprocess_function_vector_) {
	if (!preprocess_function->Run(breeze_deploy_mat)) {
	  BREEZE_DEPLOY_LOGGER_ERROR("Failed to run preprocess_function.")
	  return false;
	}
  }

  auto tensor_data = breeze_deploy_mat.GetMat().data;
  auto tensor_size = breeze_deploy_mat.GetMatDataByteSize();
  auto tensor_data_type = breeze_deploy_mat.GetMatDataType();
  input_tensor_vector_[0].SetTensorData(tensor_data, tensor_size, tensor_data_type);
  return true;
}
bool YOLOV5::Postprocess() {

  return BreezeDeployModel::Postprocess();
}
}
}