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
#include <cmath>
#include <Eigen/Dense>
#include "breeze_deploy/postprocess_function/softmax/softmax.h"
namespace breeze_deploy {
namespace function {
// Todo: Move Softmax to TopK
bool Softmax::Run(BreezeDeployTensor &tensor, ClassificationResults &result) {
  return BreezeDeployPostprocessFunction::Run(tensor, result);
}
template<typename T>
float Softmax::Run(T x, T *input_pointer, size_t input_size) {
  Eigen::Map<Eigen::VectorXf> inputVector(input_pointer, static_cast<long>(input_size));
  auto max_num = inputVector.maxCoeff();
  Eigen::VectorXf softmax = (inputVector.array() - max_num).exp();
  return exp(x - max_num) / softmax.sum();
}
template<typename T>
float Softmax::Run(T x, std::vector<T> input_vector) {
  Eigen::Map<Eigen::VectorXf> inputVector(input_vector.data(), static_cast<long>(input_vector.size()));
  auto max_num = inputVector.maxCoeff();
  Eigen::VectorXf softmax = (inputVector.array() - max_num).exp();
  return exp(x - max_num) / softmax.sum();
}
}
}