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
#include <deque>

namespace breeze_deploy {
namespace models {
struct ClassificationResult {
  ClassificationResult(std::string temp_label, size_t temp_index, float temp_confidence)
	  : label{std::move(temp_label)}, index{temp_index}, confidence{temp_confidence} {}
  std::string label;
  size_t index;
  float confidence;
};
class ClassificationResults {
 public:
  ClassificationResults() = default;

  void Clear();
  void EmplaceBack(size_t index, float confidence);
  void EmplaceFront(size_t index, float confidence);
  std::vector<ClassificationResult> GetClassificationResultVector() const;
  const std::deque<float> &GetConfidenceDeque();
  const std::deque<size_t> &GetIndexDeque();
  const std::deque<std::string> &GetLabelDeque();
  size_t GetResultSize() const;
  bool ReadLabelFile(const std::string &label_file_path);

 private:
  std::deque<std::string> label_deque_{};
  std::deque<size_t> index_deque_{};
  std::deque<float> confidence_deque_{};
  std::deque<std::string> base_labels_{};
};
}
}

#endif //BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_RESULT_H_
