// Copyright (c) 2024/2/1 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_RESULT_H_
#define BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_RESULT_H_
#include "opencv2/opencv.hpp"
namespace breeze_deploy {
namespace models {
struct ClassificationResult {
  std::vector<int64_t> topk_label_id_vector;
  std::vector<float> topk_confidence_vector;

  size_t GetSize() const { return topk_label_id_vector.size(); }
  bool Empty() const { return topk_label_id_vector.empty(); }
  void Clear() {
    topk_label_id_vector.clear();
    topk_confidence_vector.clear();
  }
};
struct DetectionResult {
  DetectionResult() = default;
  std::vector<int64_t> label_id_vector;
  std::vector<float> confidence_vector;
  std::vector<cv::Rect> rect_vector;

  std::vector<std::vector<cv::Point>> landmarks_vector;
  size_t landmarks_per_face = 0;

  size_t GetSize() const { return label_id_vector.size(); }
  bool Empty() const { return label_id_vector.empty(); }
  void Reserve(size_t size) {
    label_id_vector.reserve(size);
    confidence_vector.reserve(size);
    rect_vector.reserve(size);
    landmarks_vector.reserve(size);
  }
  void Clear() {
    label_id_vector.clear();
    confidence_vector.clear();
    rect_vector.clear();
    landmarks_vector.clear();
    landmarks_per_face = 0;
  }
  DetectionResult GetMaxConfidenceResult() {
    if (confidence_vector.empty()) {
      return {};
    }

    auto max_it = std::max_element(confidence_vector.begin(), confidence_vector.end());
    auto max_index = std::distance(confidence_vector.begin(), max_it);

    DetectionResult detection_result;
    detection_result.confidence_vector = {this->confidence_vector[max_index]};
    detection_result.label_id_vector = {this->label_id_vector[max_index]};
    detection_result.rect_vector = {this->rect_vector[max_index]};
    detection_result.landmarks_per_face = {this->landmarks_per_face};
    if (!this->landmarks_vector.empty()) {
      detection_result.landmarks_vector = this->landmarks_vector;
    }
    return std::move(detection_result);
  }
};
struct FeatureResult {
  std::vector<float> feature_vector;
  size_t GetFeatureLength() const { return feature_vector.size(); }
};
struct ImageRecognitionResult {
  // 一个识别结果只会有一个DetectionResult
  DetectionResult detection_result;

  // 一个DetectionResult内有多个目标，每个目标都有一个ClassificationResult结果
  std::vector<ClassificationResult> classification_results;

  size_t GetSize() const { return detection_result.label_id_vector.size(); }
  bool Empty() const { return detection_result.label_id_vector.empty(); }
  void Clear() {
    detection_result.Clear();
    classification_results.clear();
  }
};
}
}
#endif //BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_RESULT_H_
