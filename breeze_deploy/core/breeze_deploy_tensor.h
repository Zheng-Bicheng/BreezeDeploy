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

#ifndef BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_TENSOR_H_
#define BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_TENSOR_H_
#include <opencv2/opencv.hpp>
#include "breeze_deploy/core/breeze_deploy_type.h"

namespace breeze_deploy {
class BreezeDeployTensor {
 public:
  BreezeDeployTensor();
//  explicit BreezeDeployTensor(uint8_t *tensor_data_ptr, size_t tensor_data_size);

  void SetTensorData(uint8_t *tensor_data_ptr, size_t tensor_data_size, BreezeDeployDataType tensor_data_type);
  uint8_t *GetTensorDataPointer();
  size_t GetTensorSize() const;
  size_t GetTensorDataByteSize() const;

 private:
  uint8_t *tensor_data_ptr_ = nullptr;
  size_t tensor_data_ptr_size_ = 0;
  BreezeDeployDataType tensor_data_type_ = BreezeDeployDataType::UINT8;
};
}
#endif //BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_TENSOR_H_
