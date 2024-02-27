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

#include "breeze_deploy/models/preprocess/letterbox/letter_box.h"
namespace breeze_deploy {
namespace preprocess {
LetterBox::LetterBox(int width, int height, std::array<float, 3> scalar)
	: width_{width}, height_{height}, scalar_{scalar} {
}

// Copy form https://flyfish.blog.csdn.net/article/details/130320915
bool LetterBox::Run(BreezeDeployMat &breeze_deploy_mat) {
  BREEZE_DEPLOY_LOGGER_DEBUG("{}::Run Start",FunctionName())
  auto &src = breeze_deploy_mat.GetMat();

  // Get radio
  auto mat_height = breeze_deploy_mat.GetHeight();
  auto mat_width = breeze_deploy_mat.GetWidth();
  radio_ = std::min(static_cast<double>(width_) / mat_width, static_cast<double>(height_) / mat_height);

  // Do resize
  int inside_w = static_cast<int>(round(mat_width * radio_));
  int inside_h = static_cast<int>(round(mat_height * radio_));
  cv::resize(src, src, cv::Size(inside_w, inside_h));
  pad_width_ = (width_ - inside_w) / 2;
  pad_height_ = (height_ - inside_h) / 2;

  cv::Mat output_image(height_, width_, src.type(), cv::Scalar(scalar_[0], scalar_[1], scalar_[2])); // 用灰色进行填充

  cv::Rect roi(pad_width_, pad_height_, src.cols, src.rows);
  src.copyTo(output_image(roi));
  src = output_image;
  cv::imwrite("letter_box.png", src);
  BREEZE_DEPLOY_LOGGER_DEBUG("{}::Run End",FunctionName())
  return true;
}
}
}
