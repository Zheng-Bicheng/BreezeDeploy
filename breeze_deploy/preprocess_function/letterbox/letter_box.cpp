// Copyright (c) 2024/1/22 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/preprocess_function/letterbox/letter_box.h"
namespace breeze_deploy {
namespace function {
LetterBox::LetterBox(int width, int height)
	: width_{width}, height_{height} {
}

// Copy form https://flyfish.blog.csdn.net/article/details/130320915
bool LetterBox::Run(BreezeDeployMat &breeze_deploy_mat) {
  auto &src = breeze_deploy_mat.GetMat();
  auto mat_height = breeze_deploy_mat.GetHeight();
  auto mat_width = breeze_deploy_mat.GetWidth();

  auto scale = std::min(static_cast<double>(height_) / mat_height, static_cast<double>(width_) / mat_width);
  int inside_w = static_cast<int>(round(mat_width * scale));
  int inside_h = static_cast<int>(round(mat_height * scale));
  cv::resize(src, src, cv::Size(inside_w, inside_h));

  int pad_width = (width_ - inside_w) / 2;
  int pad_height = (height_ - inside_h) / 2;
  int top = int(round(pad_height - 0.1));
  int bottom = int(round(pad_height + 0.1));
  int left = int(round(pad_width - 0.1));
  int right = int(round(pad_width + 0.1));
  cv::copyMakeBorder(src, src, top, bottom, left, right, 0, cv::Scalar(114.0, 114.0, 114.0));
  return true;
}
}
}
