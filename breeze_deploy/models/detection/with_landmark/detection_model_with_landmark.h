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

#ifndef BREEZE_DEPLOY_MODELS_DETECTION_WITH_LANDMARK_DETECTION_MODEL_WITH_LANDMARK_H_
#define BREEZE_DEPLOY_MODELS_DETECTION_WITH_LANDMARK_DETECTION_MODEL_WITH_LANDMARK_H_
#include "breeze_deploy/models/detection/without_landmark/detection_model_without_landmark.h"
namespace breeze_deploy {
namespace models {
class DetectionModelWithLandmark : public DetectionModelWithoutLandmark {
 public:
  DetectionModelWithLandmark(const std::string &model_path, const std::string &config_file_path)
	  : DetectionModelWithoutLandmark(model_path, config_file_path) {}
  std::string ModelName() override { return "DetectionModelWithLandmark"; }
  virtual bool Predict(const cv::Mat &input_mat, DetectionResultWithLandmark& result_with_landmark) = 0;
  static cv::Mat Draw(const cv::Mat &mat, const DetectionResultWithLandmark &detection_results);
};
}
}
#endif //BREEZE_DEPLOY_MODELS_DETECTION_WITH_LANDMARK_DETECTION_MODEL_WITH_LANDMARK_H_
