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
Resnet::Resnet() {
  int width = 224;
  int height = 224;
  preprocess_function_vector_.push_back(std::make_shared<Resize>(width, height));
  preprocess_function_vector_.push_back(std::make_shared<BGRToRGB>());
  std::vector<float> mean = {0.485f, 0.456f, 0.406f};
  std::vector<float> std = {0.229f, 0.224f, 0.225f};
  preprocess_function_vector_.push_back(std::make_shared<Normalize>(mean, std));
  preprocess_function_vector_.push_back(std::make_shared<HWCToCHW>());

  input_tensor_vector_.resize(1);
  output_tensor_vector_.resize(1);
}
Resnet::~Resnet() = default;
bool Resnet::Predict(const cv::Mat &input_mat) {
  int run_index = 10000;
  spend_time_.Start();
  for (int i = 0; i < run_index; ++i) {
	if (!Preprocess(input_mat)) {
	  return false;
	}
  }
  spend_time_.End();
  spend_time_.PrintInfo("Preprocess", 1.0f / (float)run_index);
  if (!Infer()) {
	return false;
  }
  if (!Postprocess()) {
	return false;
  }
  return true;
}
bool Resnet::Preprocess(const cv::Mat &input_mat) {
  if (input_mat.empty()) {
	return false;
  }

  BreezeDeployMat breeze_deploy_mat(input_mat);
  for (const auto &preprocess_function : preprocess_function_vector_) {
	if (!preprocess_function->Run(breeze_deploy_mat)) {
	  return false;
	}
  }

  auto tensor_data = breeze_deploy_mat.GetMat().data;
  auto tensor_size = breeze_deploy_mat.GetBytes();
  input_tensor_vector_[0].SetTensorData(tensor_data, tensor_size);
  return true;
}
bool Resnet::Infer() {
  return breeze_deploy_backend_->Infer(input_tensor_vector_, output_tensor_vector_);
}
bool Resnet::Postprocess() {
  auto output_tensor_data = reinterpret_cast<float *>(output_tensor_vector_[0].GetTensorData());
  auto output_tensor_size = output_tensor_vector_[0].GetTensorSize() / sizeof(float);
  auto max_element = std::max_element(output_tensor_data, output_tensor_data + output_tensor_size);
  auto max_element_index = max_element - output_tensor_data;
  auto confidence = Softmax::Run(*max_element, output_tensor_data, output_tensor_size);
  std::cout << "max index is " << max_element_index << ";max conf is " << confidence << std::endl;
  return true;
}
}
}
