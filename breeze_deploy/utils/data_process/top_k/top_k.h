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

#ifndef BREEZE_DEPLOY_UTILS_DATA_PROCESS_TOP_K_TOP_K_H_
#define BREEZE_DEPLOY_UTILS_DATA_PROCESS_TOP_K_TOP_K_H_

#include <vector>
#include "breeze_deploy/core/result/breeze_deploy_result.h"
#include "breeze_deploy/core/tensor/breeze_deploy_tensor.h"

namespace breeze_deploy {
namespace utils {
namespace data_process {
using namespace breeze_deploy::models;
template<typename T>
bool TopK(const void *input_tensor_pointer,
		  size_t input_tensor_size,
		  std::vector<int64_t> &output_index,
		  std::vector<float> &output_tensor,
		  size_t k,
		  T min_confidence) {
  // 使用最小堆来保存 TopK 元素，最小堆的每一个元素为{float confidence, int index}并将使用std::pair来表示
  std::priority_queue<std::pair<T, int64_t>, std::vector<std::pair<T, int64_t>>, std::greater<>> min_heap;

  // 将数据插入到最小堆中
  auto temp_input_tensor_pointer = reinterpret_cast<const T *>(input_tensor_pointer);
  for (int64_t i = 0; i < input_tensor_size; i++) {
	if (temp_input_tensor_pointer[i] < min_confidence) {
	  continue;
	}

	min_heap.emplace(temp_input_tensor_pointer[i], i);
	// 如果堆的大小超过 K，弹出堆顶元素
	if (min_heap.size() > k) {
	  min_heap.pop();
	}
  }

  while (!min_heap.empty()) {
	output_tensor.emplace_back(min_heap.top().first);
	output_index.emplace_back(min_heap.top().second);
	min_heap.pop();
  }
  std::reverse(output_index.begin(), output_index.end());
  std::reverse(output_tensor.begin(), output_tensor.end());
  return true;
}

template<typename T>
bool TopK(const std::vector<T> &input_tensor,
		  std::vector<int64_t> &output_index,
		  std::vector<float> &output_tensor,
		  size_t k,
		  T min_confidence) {
  return TopK(input_tensor.data(), input_tensor.size(), output_index, output_tensor, k, min_confidence);
}

template<typename T>
bool TopK(const BreezeDeployTensor &input_tensor,
		  ClassificationResult &classification_result,
		  size_t k,
		  T min_confidence) {
  return TopK(input_tensor.GetConstTensorDataPointer(),
			  input_tensor.GetTensorSize(),
			  classification_result.label_id_vector,
			  classification_result.topk_confidence_vector,
			  k,
			  min_confidence);
}
}
}
}
#endif //BREEZE_DEPLOY_UTILS_DATA_PROCESS_TOP_K_TOP_K_H_
