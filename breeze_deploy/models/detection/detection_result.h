// Copyright (c) 2023/12/30 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_RESULT_H_
#define BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_RESULT_H_

#include <iostream>
#include <opencv2/opencv.hpp>

#include "breeze_deploy/core/breeze_deploy_logger.h"

namespace breeze_deploy {
namespace models {
struct DetectionResult {
  DetectionResult(size_t temp_index, float temp_confidence, const cv::Rect &temp_coordinate)
	  : index{temp_index}, confidence{temp_confidence}, coordinate{temp_coordinate} {}

  void SetLabel(const std::string &temp_label) {
	label = temp_label;
  }

  void PrintResult() {
	BREEZE_DEPLOY_LOGGER_INFO("label: {}, label_id: {}, label_confidence: {}, [left,top,w,h]:[{} {} {} {}]",
							  label,
							  index,
							  confidence,
							  coordinate.x,
							  coordinate.y,
							  coordinate.width,
							  coordinate.height);
  }

  std::string label;
  size_t index = -1;
  float confidence = 0.0;
  cv::Rect coordinate = {0, 0, 0, 0};
};
}
}

#endif //BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_RESULT_H_
