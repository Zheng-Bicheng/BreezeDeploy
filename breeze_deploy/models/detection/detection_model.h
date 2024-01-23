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
#include "breeze_deploy/models/detection/detection_result.h"
namespace breeze_deploy {
namespace models {
class DetectionModel : public BreezeDeployModel {
 public:
  DetectionModel(const std::string &model_path, const std::string &config_file_path);
  static cv::Mat Draw(const cv::Mat &mat, const std::vector<DetectionResult> &detection_results);
  const std::vector<DetectionResult> &GetDetectionResults();

  void SetConfidenceThreshold(float confidence_threshold) { confidence_threshold_ = confidence_threshold; }
  void SetNMSThreshold(float nms_threshold) { nms_threshold_ = nms_threshold; }

 protected:
  std::vector<DetectionResult> detection_results_;
  bool Infer() override;
  bool ReadPostprocessYAML() override;

  // For restore original coordinates
  double radio_ = 0.0;
  int pad_width_ = 0;
  int pad_height_ = 0;

  // For postprocess
  float confidence_threshold_ = 0.5;
  int landmark_num_ = 0;

  // For nms
  float nms_threshold_ = 0.5;
};
}
}
#endif //BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_MODEL_H_