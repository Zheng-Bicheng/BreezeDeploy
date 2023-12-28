// Copyright (c) 2023/12/28 Zheng-Bicheng. All Rights Reserved.
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
#include "classification_model.h"
namespace breeze_deploy {
namespace models {
bool ClassificationModel::SetLabel(const std::string &label_file_path) {
  label_vector_.clear();
  std::ifstream input_file(label_file_path);
  if (!input_file.is_open()) {
	std::cerr << "无法打开文件: " << label_file_path << std::endl;
	return false;
  }
  std::string line;
  while (std::getline(input_file, line)) {
	label_vector_.push_back(line);
  }
  input_file.close();
  return false;
}
const std::vector<ClassificationResult> &ClassificationModel::GetClassificationResult() {
  return classification_result_vector_;
}
}
}