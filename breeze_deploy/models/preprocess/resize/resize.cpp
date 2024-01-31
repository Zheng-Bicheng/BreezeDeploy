// Copyright (c) 2023/12/27 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/preprocess/resize/resize.h"

namespace breeze_deploy {
namespace preprocess {
Resize::Resize(int width, int height)
	: width_{width}, height_{height} {}
bool Resize::Run(BreezeDeployMat &breeze_deploy_mat) {
  if (width_ == 0 || height_ == 0) {
	BREEZE_DEPLOY_LOGGER_ERROR("width_ == 0 || height_ == 0")
	return false;
  }

  if (width_ == breeze_deploy_mat.GetWidth() && height_ == breeze_deploy_mat.GetHeight()) {
	return true;
  }
  auto &mat = breeze_deploy_mat.GetMat();
  // cv::INTER_NEAREST 214911 	204187 	223560 	0.989138
  // cv::INTER_CUBIC   586599					0.998558
  // cv::INTER_LINEAR  291267	269096			0.998771
  // cv::INTER_AREA    max						0.999129
  cv::resize(mat, mat, cv::Size(width_, height_), 0, 0, cv::INTER_NEAREST);
  return true;
}
}
}
