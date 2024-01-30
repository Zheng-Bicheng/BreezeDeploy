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
struct DetectionResultWithoutLandmark {
  virtual void Clear() {
	label_id_vector.clear();
	label_confidence_vector.clear();
	rect_vector.clear();
  }
  size_t GetSize() const { return label_id_vector.size(); }

  std::vector<int> label_id_vector;  // The 'indices' parameter of the cv::dnn::NMSBoxes() function requires a std::vector<int>
  std::vector<float> label_confidence_vector;
  std::vector<cv::Rect> rect_vector;
};

struct DetectionResultWithLandmark : public DetectionResultWithoutLandmark {
  void Clear() override {
	DetectionResultWithoutLandmark::Clear();
	landmarks_vector.clear();
  }
  std::vector<std::vector<cv::Point>> landmarks_vector;
};
}
}

#endif //BREEZE_DEPLOY_MODELS_DETECTION_DETECTION_RESULT_H_
