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

#ifndef BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_MODEL_H_
#define BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_MODEL_H_
#include <utility>
#include "breeze_deploy/models/breeze_deploy_model.h"
namespace breeze_deploy {
namespace models {
struct DetectionResult {
  virtual void Clear() {
	label_id_vector.clear();
	label_confidence_vector.clear();
	rect_vector.clear();
	landmarks_vector.clear();
  }
  bool Empty() const { return label_id_vector.empty(); }
  size_t GetSize() const { return label_id_vector.size(); }
  std::vector<int> label_id_vector; // The 'indices' parameter of the cv::dnn::NMSBoxes() requires a std::vector<int>
  std::vector<float> label_confidence_vector;
  std::vector<cv::Rect> rect_vector;
  std::vector<std::vector<cv::Point>> landmarks_vector;
};
class DetectionModel : public BreezeDeployModel {
 public:
  DetectionModel(const std::string &model_path, const std::string &config_file_path)
	  : BreezeDeployModel(model_path, config_file_path) {}
  std::string ModelName() override { return "DetectionModel"; }
  void SetConfidenceThreshold(float confidence_threshold) { confidence_threshold_ = confidence_threshold; }
  void SetNMSThreshold(float nms_threshold) { nms_threshold_ = nms_threshold; }
  virtual bool Predict(const cv::Mat &input_mat, DetectionResult &detection_result) = 0;
  static cv::Mat Draw(const cv::Mat &mat, const DetectionResult &detection_results);

 protected:
  bool Preprocess(const cv::Mat &input_mat) override;
  bool ReadPostprocessYAML() override;
  bool Postprocess() override;

  // For [Resize,LetterBox]
  double radio_ = 0.0;

  // For [LetterBox]
  int pad_width_ = 0;
  int pad_height_ = 0;

  // For [DetectionResultWithoutLandmark,DetectionResultWithLandmark]
  float confidence_threshold_ = 0.5;

  // For [DetectionResultWithLandmark]
  int landmark_num_ = 0;

  // For NMS
  float nms_threshold_ = 0.5;
};
}
}
#endif //BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_MODEL_H_
