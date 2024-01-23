// Copyright (c) 2023/12/30 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/core/breeze_deploy_logger.h"
#include "breeze_deploy/postprocess_function/top_k/top_k.h"

namespace breeze_deploy {
namespace function {
TopK::TopK(size_t k) : k_{k} {}
bool TopK::Run(BreezeDeployTensor &tensor, std::vector<ClassificationResult> &result) {
  // 使用最小堆来保存 TopK 元素，最小堆的每一个元素为{float confidence, int index}并将使用std::pair来表示
  std::priority_queue<std::pair<float, size_t>, std::vector<std::pair<float, size_t>>, std::greater<>> min_heap;

  // 将数据插入到最小堆中
  auto tensor_data_ptr = reinterpret_cast<float *>(tensor.GetTensorDataPointer());
  auto tensor_data_size = tensor.GetTensorSize();
  for (size_t i = 0; i < tensor_data_size; i++) {
	min_heap.emplace(tensor_data_ptr[i], i);
	// 如果堆的大小超过 K，弹出堆顶元素
	if (min_heap.size() > k_) {
	  min_heap.pop();
	}
  }

  result.clear();
  while (!min_heap.empty()) {
	result.emplace_back(min_heap.top().second, min_heap.top().first);
	min_heap.pop();
  }
  std::reverse(result.begin(), result.end());
  return true;
}
}
}