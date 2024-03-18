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

#include "breeze_deploy/models/detection/paddle_detection/paddle_detetcion.h"
namespace breeze_deploy {
namespace models {
PaddleDetection::PaddleDetection(const std::string &model_path, const std::string &config_file_path)
    : DetectionModel(model_path, config_file_path) {}

bool PaddleDetection::ProcessWithoutNMS(DetectionResult &detection) {
  return false;
}

bool PaddleDetection::ProcessWithNMS(DetectionResult &detection) {
  return true;
}

bool PaddleDetection::Predict(const cv::Mat &input_mat, DetectionResult &detection) {
  if (!BreezeDeployModel::Predict(input_mat)) {
    return false;
  }

  return true;
}
}
}