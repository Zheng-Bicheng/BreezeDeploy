// Copyright (c) 2024/2/1 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_UTILS_DATA_PROCESS_NORMALIZE_L2_NORMALIZE_L2_H_
#define BREEZE_DEPLOY_UTILS_DATA_PROCESS_NORMALIZE_L2_NORMALIZE_L2_H_
#include <vector>
#include <Eigen/Eigen>

namespace breeze_deploy {
namespace utils {
namespace data_process {
template<typename T>
bool NormalizeL2(std::vector<T> &input_data) {
  if (input_data.empty()) {
	return false;
  }
  // 将std::vector<float> 转换为 Eigen::Map
  Eigen::Map<Eigen::VectorXf> eigen_vector(input_data.data(), input_data.size());
  // 计算L2范数
  float l2_norm = eigen_vector.norm();
  // 归一化
  eigen_vector /= l2_norm;
  return true;
}
}
}
}
#endif //BREEZE_DEPLOY_UTILS_DATA_PROCESS_NORMALIZE_L2_NORMALIZE_L2_H_
