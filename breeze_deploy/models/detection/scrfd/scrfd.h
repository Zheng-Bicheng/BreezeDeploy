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

#ifndef BREEZE_DEPLOY_BREEZE_DEPLOY_MODELS_DETECTION_SCRFD_SCRFD_H_
#define BREEZE_DEPLOY_BREEZE_DEPLOY_MODELS_DETECTION_SCRFD_SCRFD_H_

#include "breeze_deploy/models/detection/detection_model.h"
namespace breeze_deploy {
namespace models {
class BREEZE_DEPLOY_EXPORT SCRFD : public DetectionModel {
 public:
  SCRFD(const std::string &model_path, const std::string &config_file_path);
  std::string ModelName() override { return "SCRFD"; }
  bool Predict(const cv::Mat &input_mat, DetectionResult &result) override;

 protected:
  bool Preprocess(const cv::Mat &input_mat) override;

 private:
  typedef struct {
    float cx;
    float cy;
  } SCRFDPoint;
  std::unordered_map<int, std::vector<SCRFDPoint>> center_points_;

  size_t landmarks_per_face_ = 5;

  // Argument for image postprocessing step,
  // downsample strides (namely, steps) for SCRFD to generate anchors,
  // will take (8,16,32) as default values.
  std::vector<int> downsample_strides_ = {8, 16, 32};
  void GeneratePoints();
};
}
}
#endif //BREEZE_DEPLOY_BREEZE_DEPLOY_MODELS_DETECTION_SCRFD_SCRFD_H_
