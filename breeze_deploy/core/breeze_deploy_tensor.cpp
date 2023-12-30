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

#include "breeze_deploy/core/breeze_deploy_tensor.h"

namespace breeze_deploy {
BreezeDeployTensor::BreezeDeployTensor() = default;
//BreezeDeployTensor::BreezeDeployTensor(uint8_t *tensor_data_ptr, size_t tensor_data_size)
//	: tensor_data_ptr_{tensor_data_ptr}, tensor_data_ptr_size_{tensor_data_size} {
//}
void BreezeDeployTensor::SetTensorData(uint8_t *tensor_data_ptr,
									   size_t tensor_data_size,
									   BreezeDeployDataType tensor_data_type) {
  tensor_data_ptr_ = tensor_data_ptr;
  tensor_data_ptr_size_ = tensor_data_size;
  tensor_data_type_ = tensor_data_type;
}
uint8_t *BreezeDeployTensor::GetTensorDataPointer() {
  return tensor_data_ptr_;
}
size_t BreezeDeployTensor::GetTensorSize() const {
  return tensor_data_ptr_size_ / GetBreezeDeployDataTypeSize(tensor_data_type_);
}
size_t BreezeDeployTensor::GetTensorDataByteSize() const {
  return tensor_data_ptr_size_;
}
}
