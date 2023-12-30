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
#include <algorithm>
#include "breeze_deploy/models/classification/resnet/resnet.h"
#include "breeze_deploy/postprocess_function/postprocess_function.h"
namespace breeze_deploy {
namespace models {
Resnet::Resnet(const std::string &model_path, const std::string &config_file_path)
	: ClassificationModel(model_path, config_file_path) {
  input_tensor_vector_.resize(1);
  output_tensor_vector_.resize(1);
}
Resnet::~Resnet() = default;
bool Resnet::Preprocess(const cv::Mat &input_mat) {
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
bool Resnet::Infer() {
  return breeze_deploy_backend_->Infer(input_tensor_vector_, output_tensor_vector_);
}
bool Resnet::Postprocess() {
  for (const auto & i : postprocess_function_vector_) {
	i->Run(output_tensor_vector_[0], classification_results_);
  }
  return true;
}
}
}
