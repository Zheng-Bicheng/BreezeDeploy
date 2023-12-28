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

#ifndef BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_MODEL_H_
#define BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_MODEL_H_
#include <utility>

#include "breeze_deploy/models/breeze_deploy_model.h"
namespace breeze_deploy {
namespace models {
class ClassificationResult {
 public:
  ClassificationResult() = default;
  ClassificationResult(std::string label, float confidence) :
	  label_{std::move(label)}, confidence_{confidence} {};
  std::string label_;
  float confidence_ = 0;
};
class ClassificationModel : public BreezeDeployModel {
 public:
  ClassificationModel() = default;
  ~ClassificationModel() override = default;
  bool SetLabel(const std::string &label_file_path);
  const std::vector<ClassificationResult>& GetClassificationResult();

 protected:
  std::vector<std::string> label_vector_;
  std::vector<ClassificationResult> classification_result_vector_;
};
}
}
#endif //BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_MODEL_H_