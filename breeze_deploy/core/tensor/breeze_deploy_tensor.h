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

namespace breeze_deploy {
enum class BreezeDeployTensorDataType {
  UNKNOWN,
  UINT8,
  FP32
};

struct BreezeDeployTensorInfo {
  BreezeDeployTensorInfo() = default;
  std::string tensor_name;
  std::vector<int64_t> tensor_shape = {0, 0, 0, 0};
  BreezeDeployTensorDataType tensor_type = BreezeDeployTensorDataType::FP32;
};

class BreezeDeployTensor {
 public:
  BreezeDeployTensor();
  void SetTensorData(uint8_t *tensor_data_ptr,
					 const std::vector<int64_t> &tensor_shape,
					 BreezeDeployTensorDataType tensor_data_type);

  uint8_t *GetTensorDataPointer();
  const uint8_t *GetConstTensorDataPointer() const;

  size_t GetTensorSize() const;
  size_t GetTensorDataByteSize() const;
  const BreezeDeployTensorInfo &GetTensorInfo();

  static size_t GetDataTypeSize(BreezeDeployTensorDataType breeze_deploy_data_type);

 private:
  uint8_t *tensor_data_ptr_ = nullptr;
  uint64_t tensor_data_ptr_size_ = 0;
  BreezeDeployTensorInfo tensor_info_ = {"", {}, BreezeDeployTensorDataType::UNKNOWN};
};
}
#endif //BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_TENSOR_H_
