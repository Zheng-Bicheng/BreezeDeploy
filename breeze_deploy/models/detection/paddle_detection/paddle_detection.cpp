// Copyright (c) 2024/02/27 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/detection/paddle_detection/paddle_detection.h"
namespace breeze_deploy {
namespace models {
PaddleDetection::PaddleDetection(const std::string &model_path, const std::string &config_file_path)
    : DetectionModel(model_path, config_file_path) {}

bool PaddleDetection::ProcessWithoutNMS(DetectionResult &detection) {
  return false;
}

bool PaddleDetection::ProcessWithNMS(DetectionResult &result) {
  auto &tensor_0 = output_tensor_vector_[0];
  auto &tensor_1 = output_tensor_vector_[1];

  int boxes_index = 0;
  int scores_index = 1;

  // Judge the index of the input Tensor
  auto &tensor_shape_0 = tensor_0.GetTensorInfo().tensor_shape;
  auto &tensor_shape_1 = tensor_1.GetTensorInfo().tensor_shape;
  if (tensor_shape_0[1] == tensor_shape_1[2]) {
    boxes_index = 0;
    scores_index = 1;
  } else if (tensor_shape_0[2] == tensor_shape_1[1]) {
    boxes_index = 1;
    scores_index = 0;
  } else {
    BDLOGGER_ASSERT(false,
                    "The shape of boxes and scores should be [batch, boxes_num, 4], [batch, classes_num, boxes_num].")
  }

  auto &tensor_boxes = output_tensor_vector_[boxes_index];
  auto *tensor_data_boxes =
      reinterpret_cast<const float *>(tensor_boxes.GetConstTensorDataPointer());  // [x1, y1, x2, y2]
  auto &tensor_scores = output_tensor_vector_[scores_index];
  auto *tensor_data_scores = reinterpret_cast<const float *>(tensor_scores.GetConstTensorDataPointer());

  std::vector<float> temp_confidences;
  std::vector<cv::Rect> temp_boxes;
  std::vector<long> temp_class_ids;

  auto total_label_num = tensor_scores.GetTensorInfo().tensor_shape[1];
  auto total_box_num = tensor_scores.GetTensorInfo().tensor_shape[2];

  float max_score_temp = 0;
  for (size_t i = 0; i < 2 * 2100; i++) {
    if (tensor_data_scores[i] > max_score_temp) {
      max_score_temp = tensor_data_scores[i];
    }
  }

  for (int i = 0; i < total_box_num; ++i) {
    auto score_pointer_start = tensor_data_scores + i;
    auto score_pointer_end = score_pointer_start + (total_label_num - 1) * total_box_num;

    long max_label_index;
    float max_label_score = 0;
    for (auto score_pointer = score_pointer_start; score_pointer <= score_pointer_end; score_pointer += total_box_num) {
      if (*score_pointer > max_label_score) {
        max_label_index = (score_pointer - score_pointer_start) / total_box_num;
        max_label_score = *score_pointer;
      }
    }

    if (max_label_score <= confidence_threshold_) {
      continue;
    }

    temp_confidences.emplace_back(max_label_score);
    temp_class_ids.emplace_back(max_label_index);
    auto *boxes_pointer = tensor_data_boxes + 4 * i;
    temp_boxes.emplace_back(boxes_pointer[0],
                            boxes_pointer[1],
                            boxes_pointer[2] - boxes_pointer[0],
                            boxes_pointer[3] - boxes_pointer[1]);
  }

  if (temp_boxes.empty()) {
    BDLOGGER_DEBUG("No target detected");
    return true;
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
    rect.width = static_cast<int>(static_cast<double>(rect.width) / radio_width_height_[0]);
    rect.height = static_cast<int>(static_cast<double>(rect.height) / radio_width_height_[1]);
  }
  return true;
}

bool PaddleDetection::Predict(const cv::Mat &input_mat, DetectionResult &result) {
  if (!BreezeDeployModel::Predict(input_mat)) {
    return false;
  }

  return ProcessWithNMS(result);
}
}
}