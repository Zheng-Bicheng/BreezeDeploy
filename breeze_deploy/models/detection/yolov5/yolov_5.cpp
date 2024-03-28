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
bool YOLOV5::Predict(const cv::Mat &input_mat, DetectionResult &result) {
  if(!BDModel::Predict(input_mat))
  {
	return false;
  }
  result.Clear();
  auto output_data = reinterpret_cast<const float *>(output_tensor_vector_[0].GetTensorDataPointer());
  auto output_shape = output_tensor_vector_[0].GetTensorInfo().tensor_shape;  // output_shape is [1,25200,85]

  std::vector<float> temp_confidences;
  std::vector<cv::Rect> temp_boxes;
  std::vector<long> temp_class_ids;
  for (size_t i = 0; i < output_shape[1]; ++i) {
	// [x1,y1,w1,h1,box_score,conf1,....,conf80]
	auto skip = i * output_shape[2];

	// Get object score
	auto object_score_pointer = output_data + skip + 4;  // size is 1
	auto object_score = *object_score_pointer;

	// Get label_name_ score
	auto label_score_pointer = object_score_pointer + 1;  // size is 80
	auto label_num = output_shape[2] - 5;
	auto max_label_score_pointer = std::max_element(label_score_pointer, label_score_pointer + label_num);

	// 最大的类别分数*置信度
	auto max_label_score = (*max_label_score_pointer) * object_score;
	if (max_label_score <= confidence_threshold_) {
	  continue;
	}
	temp_confidences.emplace_back(max_label_score);

	// Get label_name_ id
	auto label_id = max_label_score_pointer - label_score_pointer;
	temp_class_ids.emplace_back(label_id);

	// convert from [x, y, w, h] to [left, top, w, h]
	auto box_pointer = output_data + skip;
	auto left = static_cast<int>(box_pointer[0] - (box_pointer[2] / 2.0f));
	auto top = static_cast<int>((box_pointer[1] - (box_pointer[3] / 2.0f)));
	auto width = box_pointer[2];
	auto height = box_pointer[3];
	temp_boxes.emplace_back(left, top, width, height);
  }

  std::vector<int> index_vector;
  cv::dnn::NMSBoxes(temp_boxes,
                    temp_confidences,
                    confidence_threshold_,
                    nms_threshold_,
                    index_vector);
  for (int index : index_vector) {
	result.label_ids.emplace_back(temp_class_ids[index]);
	result.rects.emplace_back(temp_boxes[index]);
	result.confidences.emplace_back(temp_confidences[index]);
  }

  // 恢复box到原坐标
  for (auto &rect : result.rects) {
    rect.x = static_cast<int>(static_cast<double>(rect.x - pad_width_height_[0]) / radio_width_height_[0]);
    rect.y = static_cast<int>(static_cast<double>(rect.y - pad_width_height_[1]) / radio_width_height_[1]);
    rect.width = static_cast<int>(static_cast<double>(rect.width) / pad_width_height_[0]);
    rect.height = static_cast<int>(static_cast<double>(rect.height) / radio_width_height_[1]);
  }
  return true;
}
}
}