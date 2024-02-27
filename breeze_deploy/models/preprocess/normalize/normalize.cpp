// Copyright (c) 2023/12/26 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/preprocess/normalize/normalize.h"
namespace breeze_deploy {
namespace preprocess {
Normalize::Normalize(const std::vector<float> &mean, const std::vector<float> &std) {
  std::vector<float> mean_(mean.begin(), mean.end());
  std::vector<float> std_(std.begin(), std.end());
  std::vector<float> min_(mean.size(), 0.0);
  std::vector<float> max_(mean.size(), 255.0);
  for (auto c = 0; c < mean_.size(); ++c) {
	auto alpha = 1.0;
	alpha /= (max_[c] - min_[c]);
	auto beta = (mean_[c] + min_[c] * alpha) / std_[c] * -1.0;
	alpha /= std_[c];
	alpha_.push_back(static_cast<float>(alpha));
	beta_.push_back(static_cast<float>(beta));
  }
}
bool Normalize::Run(BreezeDeployMat &breeze_deploy_mat) {
  auto &cv_mat = breeze_deploy_mat.GetMat();
  std::vector<cv::Mat> split_im;
  cv::split(cv_mat, split_im);
  for (int c = 0; c < cv_mat.channels(); c++) {
	split_im[c].convertTo(split_im[c], CV_32FC1, alpha_[c], beta_[c]);
  }
  cv::merge(split_im, cv_mat);
  return true;
}
}
}