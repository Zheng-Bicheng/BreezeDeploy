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
#include <fstream>
#include "breeze_deploy/core/breeze_deploy_logger.h"
#include "breeze_deploy/models/classification/classification_result.h"

namespace breeze_deploy {
namespace models {
void ClassificationResults::Clear() {
  label_vector_.clear();
  index_vector_.clear();
  confidence_vector_.clear();
}
void ClassificationResults::EmplaceBack(size_t index, float confidence) {
  label_vector_.emplace_back(base_labels_[index]);
  index_vector_.emplace_back(index);
  confidence_vector_.emplace_back(confidence);
}
size_t ClassificationResults::GetResultSize() const {
  return index_vector_.size();
}
bool ClassificationResults::ReadLabelFile(const std::string &label_file_path) {
  base_labels_.clear();
  std::ifstream input_file(label_file_path);
  if (!input_file.is_open()) {
	BREEZE_DEPLOY_LOGGER_ERROR("Could not open file: {}.", label_file_path)
	return false;
  }
  std::string line;
  while (std::getline(input_file, line)) {
	base_labels_.push_back(line);
  }
  input_file.close();
  return true;
}
const std::vector<std::string> &ClassificationResults::GetLabelVector() {
  return label_vector_;
}
const std::vector<size_t> &ClassificationResults::GetIndexVector() {
  return index_vector_;
}
const std::vector<float> &ClassificationResults::GetConfidenceVector() {
  return confidence_vector_;
}
std::vector<ClassificationResult> ClassificationResults::GetClassificationResultVector() const {
  auto result_size = GetResultSize();
  std::vector<ClassificationResult> classification_results;
  classification_results.reserve(result_size);
  for (int i = 0; i < result_size; ++i) {
	classification_results.emplace_back(label_vector_[i], index_vector_[i], confidence_vector_[i]);
  }
  return classification_results;
}
}
}