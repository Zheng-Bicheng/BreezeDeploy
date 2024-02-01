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

namespace breeze_deploy {
namespace utils {
namespace data_process {
template<typename T>
bool TopK(const std::vector<T> &input_tensor,
		  std::vector<int64_t> &output_index,
		  std::vector<float> &output_tensor,
		  size_t k,
		  T min_confidence);
}
}
}
#endif //BREEZE_DEPLOY_UTILS_DATA_PROCESS_TOP_K_TOP_K_H_
