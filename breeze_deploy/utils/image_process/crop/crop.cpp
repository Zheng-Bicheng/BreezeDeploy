// Copyright (c) 2024/1/31 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/utils/image_process/crop/crop.h"
namespace breeze_deploy {
namespace utils {
namespace image_process {
std::vector<cv::Mat> CropImage(const cv::Mat &original_image,
							   const DetectionResult &detection_result,
							   const std::string &save_path) {
  auto &rect_vector = detection_result.rects;
  if (rect_vector.empty()) {
	return {};
  }

  std::vector<cv::Mat> cropped_image_vector;
  cropped_image_vector.reserve(rect_vector.size());
  for (auto rect : rect_vector) {
	// TODO: The save_path needs to be modified to 'folder/number.png'.
	cropped_image_vector.emplace_back(CropImage(original_image, rect, save_path));
  }
  return std::move(cropped_image_vector);
}
cv::Mat CropImage(const cv::Mat &original_image, const cv::Rect &roi_rect, const std::string &save_path) {
  // 确保矩形在图像范围内
  cv::Rect valid_rect = roi_rect & cv::Rect(0, 0, original_image.cols, original_image.rows);
  // 使用ROI（Region of Interest）来裁剪图像
  cv::Mat cropped_image = original_image(valid_rect);
  if (!save_path.empty()) {
	cv::imwrite(save_path, cropped_image);
  }
  return cropped_image;
}
}
}
}