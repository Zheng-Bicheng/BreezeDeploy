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

#include "breeze_deploy/models/detection/with_landmark/detection_model_with_landmark.h"
namespace breeze_deploy {
namespace models {
cv::Mat DetectionModelWithLandmark::Draw(const cv::Mat &mat, const DetectionResultWithLandmark &detection_result) {
  DetectionModelWithoutLandmark::Draw(mat, *dynamic_cast<const DetectionResultWithoutLandmark *>(&detection_result));
  for (int i = 0; i < detection_result.GetSize(); ++i) {
	auto landmarks_vector = detection_result.landmarks_vector[i];
	for (const auto &landmark : landmarks_vector) {
	  cv::circle(mat, landmark, 1, cv::Scalar(0, 0, 255), 1);
	}
  }
  return mat;
}
}
}