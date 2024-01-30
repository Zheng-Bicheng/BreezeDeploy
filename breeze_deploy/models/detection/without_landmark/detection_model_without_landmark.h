// Copyright (c) 2024/1/30 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_MODELS_DETECTION_WITHOUT_LANDMARK_DETECTION_MODEL_WITHOUT_LANDMARK_H_
#define BREEZE_DEPLOY_MODELS_DETECTION_WITHOUT_LANDMARK_DETECTION_MODEL_WITHOUT_LANDMARK_H_
#include "breeze_deploy/models/detection/detection_model.h"
namespace breeze_deploy {
namespace models {
class DetectionModelWithoutLandmark : public DetectionModel {
 public:
  DetectionModelWithoutLandmark(const std::string &model_path, const std::string &config_file_path)
	  : DetectionModel(model_path, config_file_path) {}
  std::string ModelName() override { return "DetectionModelWithoutLandmark"; }
  virtual bool Predict(const cv::Mat &input_mat, DetectionResultWithoutLandmark& result_without_landmark) = 0;
  static cv::Mat Draw(const cv::Mat &mat, const DetectionResultWithoutLandmark &detection_results);
};
}
}
#endif //BREEZE_DEPLOY_MODELS_DETECTION_WITHOUT_LANDMARK_DETECTION_MODEL_WITHOUT_LANDMARK_H_
