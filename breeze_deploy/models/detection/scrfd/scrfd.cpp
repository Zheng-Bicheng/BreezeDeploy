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

#include "breeze_deploy/models/detection/scrfd/scrfd.h"
namespace breeze_deploy {
namespace models {
SCRFD::SCRFD(const std::string &model_path, const std::string &config_file_path)
    : DetectionModel(model_path, config_file_path) {}

void SCRFD::GeneratePoints() {
  auto input_shape = input_tensor_vector_[0].GetTensorInfo().tensor_shape;
  // 8, 16, 32
  for (auto local_stride : downsample_strides_) {
    unsigned int num_grid_h;
    unsigned int num_grid_w;
    if (input_shape[1] == 3) {
      // input_shape data_format is [N C H W].
      num_grid_h = input_shape[2] / local_stride;
      num_grid_w = input_shape[3] / local_stride;
    } else {
      // input_shape data_format is [N H W C].
      num_grid_h = input_shape[1] / local_stride;
      num_grid_w = input_shape[2] / local_stride;
    }

    for (unsigned int i = 0; i < num_grid_h; ++i) {
      for (unsigned int j = 0; j < num_grid_w; ++j) {
        constexpr size_t num_anchors = 2;
        for (unsigned int k = 0; k < num_anchors; ++k) {
          SCRFDPoint point;
          point.cx = static_cast<float>(j);
          point.cy = static_cast<float>(i);
          center_points_[local_stride].push_back(point);
        }
      }
    }
  }
}

bool SCRFD::Preprocess(const cv::Mat &input_mat) {
  if (!DetectionModel::Preprocess(input_mat)) {
    return false;
  }

  GeneratePoints();
  return true;
}

bool SCRFD::Predict(const cv::Mat &input_mat,
                    DetectionResult &result) {
  // output tensor size must be in [9,6,10,15]
  auto tensor_size = output_tensor_vector_.size();
  BDLOGGER_ASSERT((tensor_size == 9 || tensor_size == 6 || tensor_size == 9 || tensor_size == 15),
                  "The default number of output tensor must in [6, 9, 10, 15].")

  // number of downsample_strides_
  auto fmc = downsample_strides_.size();
  BDLOGGER_ASSERT((fmc == 3 || fmc == 5), "The fmc must be 3 or 5")

  if (!BDModel::Predict(input_mat)) {
    BDLOGGER_ERROR("Failed to preprocess input mat.")
    return false;
  }

  // Only support when batch is 1.
  auto output_batch = output_tensor_vector_[0].GetTensorInfo().tensor_shape[0];
  BDLOGGER_ASSERT((output_batch == 1), "Only support batch =1 now.")

  result.Clear();

  // must be setup landmarks_per_face_ before reserve
  auto use_kps = (output_tensor_vector_.size() == 9);
  if (use_kps) {
    result.landmarks_per_face = landmarks_per_face_;
  }

  std::vector<float> temp_confidences;
  std::vector<cv::Rect> temp_boxes;
  std::vector<std::vector<cv::Point>> temp_landmark_vector;
  // loop each stride
  for (int f = 0; f < fmc; ++f) {
    auto bbox_tensor = output_tensor_vector_.at(f + fmc);
    auto bbox_ptr = reinterpret_cast<float *>(bbox_tensor.GetTensorDataPointer());

    auto score_tensor = output_tensor_vector_.at(f);
    auto score_ptr = reinterpret_cast<float *>(score_tensor.GetTensorDataPointer());

    auto num_points = score_tensor.GetTensorInfo().tensor_shape[1];
    int current_stride = downsample_strides_[f];
    auto &stride_points = center_points_[current_stride];
    // loop each anchor
    for (unsigned int i = 0; i < num_points; ++i) {
      const float cls_conf = score_ptr[i];
      if (cls_conf < confidence_threshold_) {
        continue;  // filter
      }

      temp_confidences.emplace_back(cls_conf);

      // Get box
      auto &point = stride_points.at(i);
      const float cx = point.cx;  // cx
      const float cy = point.cy;  // cy
      const float *offsets = bbox_ptr + i * 4;
      float l = offsets[0];  // left
      float t = offsets[1];  // top
      float r = offsets[2];  // right
      float b = offsets[3];  // bottom
      auto left = (cx - l) * static_cast<float>(current_stride);
      auto top = (cy - t) * static_cast<float>(current_stride);
      auto right = (cx + r) * static_cast<float>(current_stride);
      auto bottom = (cy + b) * static_cast<float>(current_stride);
      auto width = right - left;
      auto height = bottom - top;
      temp_boxes.emplace_back(left, top, width, height);

      if (use_kps) {
        auto landmarks_tensor = output_tensor_vector_.at(f + 2 * fmc);
        auto landmarks_ptr = reinterpret_cast<float *>(landmarks_tensor.GetTensorDataPointer());
        // landmarks
        std::vector<cv::Point> landmark(landmark_num_);
        const float *kps_offsets = landmarks_ptr + i * (landmarks_per_face_ * 2);
        for (unsigned int j = 0; j < landmarks_per_face_ * 2; j += 2) {
          float kps_l = kps_offsets[j];
          float kps_t = kps_offsets[j + 1];
          auto kps_x = (cx + kps_l) * static_cast<float>(current_stride);
          auto kps_y = (cy + kps_t) * static_cast<float>(current_stride);
          landmark.emplace_back(kps_x, kps_y);
        }
        temp_landmark_vector.emplace_back(landmark);
      }
    }
  }

  if (temp_confidences.empty()) {
    return true;
  }

  std::vector<int> index_vector;
  cv::dnn::NMSBoxes(temp_boxes,
                    temp_confidences,
                    confidence_threshold_,
                    nms_threshold_,
                    index_vector);

  for (int index : index_vector) {
    result.label_ids.emplace_back(0);
    result.rects.emplace_back(temp_boxes[index]);
    result.confidences.emplace_back(temp_confidences[index]);
    if (use_kps) {
      result.landmarks.emplace_back(temp_landmark_vector[index]);
    }
  }

  // 恢复box到原坐标
  for (auto &rect : result.rects) {
    rect.x = static_cast<int>(static_cast<double>(rect.x - pad_width_height_[0]) / radio_width_height_[0]);
    rect.y = static_cast<int>(static_cast<double>(rect.y - pad_width_height_[1]) / radio_width_height_[1]);
    rect.width = static_cast<int>(static_cast<double>(rect.width) / pad_width_height_[0]);
    rect.height = static_cast<int>(static_cast<double>(rect.height) / radio_width_height_[1]);
  }

  // 恢复landmark到原坐标
  for (auto &landmarks : result.landmarks) {
    for (auto &landmark : landmarks) {
      landmark.x = static_cast<int>(static_cast<double>(landmark.x - pad_width_height_[0]) / pad_width_height_[0]);
      landmark.y = static_cast<int>(static_cast<double>(landmark.y - pad_width_height_[1]) / radio_width_height_[1]);
    }
  }
  return true;
}
}
}