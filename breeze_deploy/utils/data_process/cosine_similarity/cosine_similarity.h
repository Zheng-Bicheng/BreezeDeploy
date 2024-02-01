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

#ifndef BREEZE_DEPLOY_UTILS_DATA_PROCESS_COSINE_SIMILARITY_H_
#define BREEZE_DEPLOY_UTILS_DATA_PROCESS_COSINE_SIMILARITY_H_
#include <Eigen/Eigen>
#include "breeze_deploy/core/result/breeze_deploy_result.h"
namespace breeze_deploy {
namespace utils {
namespace data_process {
using namespace breeze_deploy::models;
template<typename T>
double CosineSimilarity(const std::vector<T> &a, const std::vector<T> &b) {
  if ((a.size() != b.size()) && (a.empty()) || (b.empty())) {
	BREEZE_DEPLOY_LOGGER_ERROR(
		"The size of Vector A and B must be equal and greater than 0. But the size of vector A is {}, while the size of vector B is also {}.",
		a.size(), b.size())
	return 0;
  }

  Eigen::Map<const Eigen::VectorXf> eigen_vector_a(a.data(), static_cast<long>(a.size()));
  Eigen::Map<const Eigen::VectorXf> eigen_vector_b(b.data(), static_cast<long>(b.size()));
  return eigen_vector_a.dot(eigen_vector_b) / (eigen_vector_a.norm() * eigen_vector_b.norm());
}

double CosineSimilarity(const FeatureResult &a, const FeatureResult &b) {
  return CosineSimilarity<float>(a.feature_vector, b.feature_vector);
}
}
}
}
#endif //BREEZE_DEPLOY_UTILS_DATA_PROCESS_COSINE_SIMILARITY_H_
