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

#ifndef BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_MAT_H_
#define BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_MAT_H_
#include "opencv2/opencv.hpp"
#include "breeze_deploy/core/tensor/breeze_deploy_tensor.h"
namespace breeze_deploy {
enum class BreezeDeployMatFormat{
  UNKNOWN,
  CHW,
  HWC
};
class BreezeDeployMat {
 public:
  BreezeDeployMat() = default;
  explicit BreezeDeployMat(const cv::Mat &mat);

  cv::Mat &GetMat();

  // C H W
  int GetChannel() const;
  int GetWidth() const;
  int GetHeight() const;

  // Type
  int GetMatType();

  // Data Type
  size_t GetMatDataByteSize() const;
  BDTensorType GetMatDataType();

  // Data Format
  BreezeDeployMatFormat GetMatDataFormat() const;
  void SetDataFormat(BreezeDeployMatFormat format);
 private:
  cv::Mat mat_{};
  BreezeDeployMatFormat format_ = BreezeDeployMatFormat::HWC;
};
}
#endif //BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_MAT_H_
