// Copyright (c) 2024/1/30 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_MODELS_CLASSIFICATION_POSTPROCESS_FUNCTION_H_
#define BREEZE_DEPLOY_MODELS_CLASSIFICATION_POSTPROCESS_FUNCTION_H_

#include <Eigen/Dense>
#include "breeze_deploy/models/classification/classification_result.h"

namespace breeze_deploy {
namespace models {
template<typename T>
class TopK {
 public:
  static bool Run(BreezeDeployTensor &tensor,
				  ClassificationLabelResult &label_result,
				  size_t k,
				  T min_confidence) {
	// 使用最小堆来保存 TopK 元素，最小堆的每一个元素为{float confidence, int index}并将使用std::pair来表示
	std::priority_queue<std::pair<T, size_t>, std::vector<std::pair<T, size_t>>, std::greater<>> min_heap;

	// 将数据插入到最小堆中
	auto tensor_data_ptr = reinterpret_cast<T *>(tensor.GetTensorDataPointer());
	auto tensor_data_size = tensor.GetTensorSize();
	for (size_t i = 0; i < tensor_data_size; i++) {
	  if (tensor_data_ptr[i] < min_confidence) {
		continue;
	  }

	  min_heap.emplace(tensor_data_ptr[i], i);
	  // 如果堆的大小超过 K，弹出堆顶元素
	  if (min_heap.size() > k) {
		min_heap.pop();
	  }
	}

	auto &label_id_vector = label_result.label_id_vector;
	auto &confidence_vector = label_result.confidence_vector;
	while (!min_heap.empty()) {
	  confidence_vector.emplace_back(min_heap.top().first);
	  label_id_vector.emplace_back(min_heap.top().second);
	  min_heap.pop();
	}
	std::reverse(label_id_vector.begin(), label_id_vector.end());
	std::reverse(confidence_vector.begin(), confidence_vector.end());
	return true;
  }
};

template<typename T>
class Softmax {
 public:
  static bool Run(BreezeDeployTensor &tensor, ClassificationLabelResult &result) {
#if 0
	auto tensor_data_ptr = reinterpret_cast<float *>(tensor.GetTensorDataPointer());
	auto tensor_data_size = tensor.GetTensorSize();
	Eigen::Map<Eigen::VectorXf> input(tensor_data_ptr, static_cast<long>(tensor_data_size));
	auto max_num = input.maxCoeff();
	Eigen::VectorXf softmax = (input.array() - max_num).exp();
	return exp(x - max_num) / softmax.sum();
#endif
	return false;
  }
};

}
}
#endif //BREEZE_DEPLOY_MODELS_CLASSIFICATION_POSTPROCESS_FUNCTION_H_
