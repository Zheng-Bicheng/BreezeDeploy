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

#ifndef BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_RESULT_H_
#define BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_RESULT_H_

#include <iostream>

namespace breeze_deploy {
namespace models {
struct ClassificationFeatureResult {
  size_t GetSize() { return feature_vector_.size(); }
  std::vector<float> feature_vector_;
};
struct ClassificationLabelResult {
  std::vector<std::string> label_name_vector;
  std::vector<size_t> label_id_vector;
  std::vector<float> confidence_vector;
  void Clear() {
	label_name_vector.clear();
	label_id_vector.clear();
	confidence_vector.clear();
  }
  size_t GetSize() { return label_id_vector.size(); }
};
}
}

#endif //BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_RESULT_H_
