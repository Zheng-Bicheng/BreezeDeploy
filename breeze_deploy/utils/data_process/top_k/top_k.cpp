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

#include "breeze_deploy/utils/data_process/top_k/top_k.h"

#include <queue>

namespace breeze_deploy {
namespace utils {
namespace data_process {
template<typename T>
bool TopK(const std::vector<T> &input_tensor,
		  std::vector<int64_t> &output_index,
		  std::vector<T> &output_tensor,
		  size_t k,
		  T min_confidence) {
  // 使用最小堆来保存 TopK 元素，最小堆的每一个元素为{float confidence, int index}并将使用std::pair来表示
  std::priority_queue<std::pair<T, int64_t>, std::vector<std::pair<T, int64_t>>, std::greater<>> min_heap;

  // 将数据插入到最小堆中
  for (int64_t i = 0; i < input_tensor.size(); i++) {
	if (input_tensor[i] < min_confidence) {
	  continue;
	}

	min_heap.emplace(input_tensor[i], i);
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
}
}
}