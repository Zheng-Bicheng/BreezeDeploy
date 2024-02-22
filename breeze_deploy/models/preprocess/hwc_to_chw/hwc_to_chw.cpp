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

#include "breeze_deploy/models/preprocess/hwc_to_chw/hwc_to_chw.h"
namespace breeze_deploy {
namespace preprocess {
bool HWCToCHW::Run(BreezeDeployMat &breeze_deploy_mat) {
  BREEZE_DEPLOY_LOGGER_DEBUG("{}::Run Start",FunctionName())
  auto &hwc_mat = breeze_deploy_mat.GetMat();
  auto channel_mat_vector = std::vector<cv::Mat>(3);
  cv::split(hwc_mat, channel_mat_vector);
  for (int i = 0; i < channel_mat_vector.size(); ++i) {
	auto &channel_mat = channel_mat_vector[i];
	auto channel_data = std::vector<float>(channel_mat.reshape(1, 1));
	auto hwc_mat_pointer = hwc_mat.data + i * channel_data.size() * sizeof(float);
	memcpy(hwc_mat_pointer, channel_data.data(), channel_data.size() * sizeof(float));
  }
  breeze_deploy_mat.SetDataFormat(BreezeDeployMatFormat::CHW);
  BREEZE_DEPLOY_LOGGER_DEBUG("{}::Run End",FunctionName())
  return true;
}
}
}