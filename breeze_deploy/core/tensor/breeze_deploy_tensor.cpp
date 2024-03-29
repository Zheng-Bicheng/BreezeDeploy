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

#include "breeze_deploy_tensor.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"

namespace breeze_deploy {
BreezeDeployTensor::BreezeDeployTensor() = default;
void BreezeDeployTensor::SetTensorData(uint8_t *tensor_data_ptr,
                                       const std::vector<int64_t> &tensor_shape,
                                       BreezeDeployTensorType tensor_data_type) {
  tensor_data_ptr_ = tensor_data_ptr;
  tensor_info_.tensor_shape = tensor_shape;
  tensor_info_.tensor_type = tensor_data_type;

  tensor_data_ptr_size_ = 1;
  for (auto shape : tensor_shape) {
    tensor_data_ptr_size_ *= shape;
  }
}
uint8_t *BreezeDeployTensor::GetTensorDataPointer() {
  return tensor_data_ptr_;
}
const uint8_t *BreezeDeployTensor::GetConstTensorDataPointer() const {
  return tensor_data_ptr_;
}
size_t BreezeDeployTensor::GetTensorSize() const {
  return tensor_data_ptr_size_;
}
size_t BreezeDeployTensor::GetTensorDataByteSize() const {
  return tensor_data_ptr_size_ * GetBDTensorTypeSize(tensor_info_.tensor_type);
}
const BreezeDeployTensorInfo &BreezeDeployTensor::GetTensorInfo() {
  return tensor_info_;
}
size_t GetBDTensorTypeSize(BreezeDeployTensorType breeze_deploy_data_type) {
  size_t size = 0;
  switch (breeze_deploy_data_type) {
    case BreezeDeployTensorType::UINT8:size = sizeof(uint8_t);
      break;
    case BreezeDeployTensorType::FP32:size = sizeof(float);
      break;
    case BreezeDeployTensorType::INT8:size = sizeof(int8_t);
      break;
    case BreezeDeployTensorType::INT16:size = sizeof(int16_t);
      break;
    case BreezeDeployTensorType::INT32:size = sizeof(int32_t);
      break;
    case BreezeDeployTensorType::INT64:size = sizeof(int64_t);
      break;
    case BreezeDeployTensorType::BOOL:size = sizeof(bool);
      break;
    case BreezeDeployTensorType::FP16:
      size = sizeof(cv::float16_t);
      break;
    case BreezeDeployTensorType::UNKNOWN:size = 0;
      break;
  }
  return size;
}
}
