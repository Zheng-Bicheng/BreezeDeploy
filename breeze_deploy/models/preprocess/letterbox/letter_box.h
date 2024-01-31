// Copyright (c) 2024/1/22 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_PREPROCESS_FUNCTION_LETTERBOX_LETTER_BOX_H_
#define BREEZE_DEPLOY_PREPROCESS_FUNCTION_LETTERBOX_LETTER_BOX_H_
#include "breeze_deploy/models/preprocess/breeze_deploy_preprocess.h"
namespace breeze_deploy {
namespace preprocess {
class LetterBox : public BreezeDeployPreprocess {
 public:
  LetterBox(int width, int height, std::array<float, 3> scalar = {114, 114, 114});
  std::string FunctionName() override { return "LetterBox"; }
  bool Run(BreezeDeployMat &breeze_deploy_mat) override;

 private:
  int width_ = 0;
  int height_ = 0;

  std::array<float, 3> scalar_ = {0, 0, 0};
};
}
}
#endif //BREEZE_DEPLOY_PREPROCESS_FUNCTION_LETTERBOX_LETTER_BOX_H_
