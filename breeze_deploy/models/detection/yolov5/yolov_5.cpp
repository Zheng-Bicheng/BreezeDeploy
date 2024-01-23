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

bool YOLOV5::Postprocess() {
  detection_results_.clear();

  auto output_data = reinterpret_cast<float *>(output_tensor_vector_[0].GetTensorDataPointer());
  // output_shape is [1,25200,85]
  auto output_shape = output_tensor_vector_[0].GetTensorInfo().tensor_shape;

  // TODO: Remove conf_threshold_
  float conf_threshold_ = 0.1;
  std::vector<float> confidences;
  std::vector<cv::Rect> boxes;
  std::vector<long> class_ids;
  for (size_t i = 0; i < output_shape[1]; ++i) {
	// [x1,y1,w1,h1,box_score,conf1,....,conf80]
	auto skip = i * output_shape[2];

	// Get object score
	auto object_score_pointer = output_data + skip + 4;  // size is 1
	auto object_score = output_data[skip + 4];

	// Get label_name_ score
	auto label_score_pointer = object_score_pointer + 1;  // size is 80
	auto label_num = output_shape[2] - 5;
	auto max_label_score_pointer = std::max_element(label_score_pointer, label_score_pointer + label_num);
	// 最大的类别分数*置信度
	auto max_label_score = (*max_label_score_pointer) * object_score;
	if (max_label_score <= conf_threshold_) {
	  continue;
	}
	confidences.emplace_back(max_label_score);

	// Get label_name_ id
	auto label_id = max_label_score_pointer - label_score_pointer;
	class_ids.emplace_back(label_id);

	// convert from [x, y, w, h] to [left, top, w, h]
	auto box_pointer = output_data + skip;
	std::array<float, 4> box = {box_pointer[0] - box_pointer[2] / 2.0f, box_pointer[1] - box_pointer[3] / 2.0f,
								box_pointer[2], box_pointer[3]};
	boxes.emplace_back(box_pointer[0] - box_pointer[2] / 2.0f, box_pointer[1] - box_pointer[3] / 2.0f,
					   box_pointer[0] + box_pointer[2] / 2.0f, box_pointer[1] + box_pointer[3] / 2.0f);
  }

  std::vector<int> indices;
  // TODO:
//  cv::dnn::NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);
  cv::dnn::NMSBoxes(boxes, confidences, conf_threshold_, 0.5, indices);
  for (int idx : indices) {
	detection_results_.emplace_back(class_ids[idx], confidences[idx], boxes[idx]);
  }
  return true;
}
}
}