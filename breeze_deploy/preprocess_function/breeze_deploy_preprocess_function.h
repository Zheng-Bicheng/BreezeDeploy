// Copyright (c) 2023/12/26 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_MODELS_PREPROCESS_FUNCTION_BASE_PREPROCESS_FUNCTION_H_
#define BREEZE_DEPLOY_MODELS_PREPROCESS_FUNCTION_BASE_PREPROCESS_FUNCTION_H_

#include <iostream>
#include "breeze_deploy/core/breeze_deploy_mat.h"
#include "breeze_deploy/core/breeze_deploy_logger.h"

namespace breeze_deploy {
namespace function {
class BreezeDeployPreprocessFunction {
 public:
  virtual std::string FunctionName() = 0;
  virtual bool Run(BreezeDeployMat &breeze_deploy_mat) = 0;
  double GetRadio() { return radio_; }
  int GetPadWidth() { return pad_width_; }
  int GetPadHeight() { return pad_height_; }

 protected:
  // For resize
  double radio_ = 0.0;

  // For padding
  int pad_width_ = 0;
  int pad_height_ = 0;
};
}
}
#endif //BREEZE_DEPLOY_MODELS_PREPROCESS_FUNCTION_BASE_PREPROCESS_FUNCTION_H_
