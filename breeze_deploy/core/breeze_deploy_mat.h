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
#include <opencv2/opencv.hpp>
#include "breeze_deploy/core/breeze_deploy_type.h"
namespace breeze_deploy {
class BreezeDeployMat {
 public:
  explicit BreezeDeployMat(const cv::Mat &mat);

  cv::Mat &GetMat();

  int GetWidth() const;
  void SetWidth(int width);

  int GetHeight() const;
  void SetHeight(int height);

  int GetChannel() const;
  void SetChannel(int channel);

  size_t GetMatDataByteSize() const;
  BreezeDeployDataType GetMatDataType();
 private:
  cv::Mat mat_{};
  int channel_ = 0;
  int width_ = 0;
  int height_ = 0;
};
}
#endif //BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_MAT_H_
