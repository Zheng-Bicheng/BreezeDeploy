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
  DetectionResult(size_t temp_label_id, float temp_confidence, const cv::Rect &temp_coordinate)
	  : label_id_{temp_label_id}, label_confidence_{temp_confidence}, rect_{temp_coordinate} {
  }
  DetectionResult(size_t temp_label_id,
				  float temp_confidence,
				  const cv::Rect &temp_coordinate,
				  const std::vector<cv::Point> &landmark)
	  : label_id_{temp_label_id}, label_confidence_{temp_confidence}, rect_{temp_coordinate}, landmarks_{landmark} {
  }

  void PrintResult() {
	BREEZE_DEPLOY_LOGGER_INFO("label_id: {}, label_confidence_: {}, [left,top,w,h]:[{} {} {} {}]",
							  label_id_,
							  label_confidence_,
							  rect_.x,
							  rect_.y,
							  rect_.width,
							  rect_.height)
  }

  size_t label_id_ = -1;
  float label_confidence_ = 0.0;
  cv::Rect rect_ = {0, 0, 0, 0};
  std::vector<cv::Point> landmarks_;
};
}
}

#endif //BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_RESULT_H_
