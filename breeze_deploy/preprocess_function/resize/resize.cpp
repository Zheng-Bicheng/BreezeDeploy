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

#include "breeze_deploy/preprocess_function/resize/resize.h"
breeze_deploy::function::Resize::Resize(int width, int height)
	: width_{width}, height_{height} {
}
breeze_deploy::function::Resize::~Resize() = default;
bool breeze_deploy::function::Resize::Run(BreezeDeployMat &tensor) {
  if (width_ == 0 || height_ == 0) {
	return false;
  }

  if (width_ == tensor.GetWidth() && height_ == tensor.GetHeight()) {
	return true;
  }
  auto &mat = tensor.GetMat();
  // cv::INTER_NEAREST 214911 	204187 	223560 	0.989138
  // cv::INTER_CUBIC   586599					0.998558
  // cv::INTER_LINEAR  291267	269096			0.998771
  // cv::INTER_AREA    max						0.999129
  cv::resize(mat, mat, cv::Size(width_, height_), 0, 0, cv::INTER_NEAREST);
  tensor.SetHeight(height_);
  tensor.SetWidth(width_);
  return true;
}