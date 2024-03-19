// Copyright (c) 2024/02/22 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_BREEZE_DEPLOY_UTILS_DATA_PROCESS_SOFTMAX_SOFTMAX_H_
#define BREEZE_DEPLOY_BREEZE_DEPLOY_UTILS_DATA_PROCESS_SOFTMAX_SOFTMAX_H_
#include "Eigen/Eigen"
#include "breeze_deploy/core/result/breeze_deploy_result.h"
namespace breeze_deploy {
namespace utils {
namespace data_process {
// Softmax不支持int数据类型，仅支持fp数据类型
BREEZE_DEPLOY_EXPORT bool Softmax(float *data, long length) {
  if (length == 0) {
    BDLOGGER_ERROR("The Vector A is empty.")
    return false;
  }
  Eigen::Map<Eigen::VectorXf> eigen_vector_a(data, length);
  auto max_num = eigen_vector_a.maxCoeff();
  auto exp_values = eigen_vector_a.array().exp();
  auto exp_sum = exp_values.sum();
  eigen_vector_a = exp_values / exp_sum;
  return true;
}

BREEZE_DEPLOY_EXPORT bool Softmax(std::vector<float> &data) {
  return Softmax(data.data(), static_cast<long>(data.size()));
}

BREEZE_DEPLOY_EXPORT bool Softmax(BreezeDeployTensor &tensor) {
  auto tensor_pointer = reinterpret_cast<float *>(tensor.GetTensorDataPointer());
  auto tensor_size = static_cast<long>(tensor.GetTensorSize());
  return Softmax(tensor_pointer, tensor_size);
}
}
}
}
#endif //BREEZE_DEPLOY_BREEZE_DEPLOY_UTILS_DATA_PROCESS_SOFTMAX_SOFTMAX_H_
