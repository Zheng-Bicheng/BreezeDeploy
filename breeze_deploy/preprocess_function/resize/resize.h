// Copyright (c) 2023/12/27 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_PREPROCESS_FUNCTION_RESIZE_RESIZE_H_
#define BREEZE_DEPLOY_PREPROCESS_FUNCTION_RESIZE_RESIZE_H_
#include <vector>
#include "breeze_deploy/preprocess_function/breeze_deploy_preprocess_function.h"

namespace breeze_deploy {
namespace function {
class Resize : public BreezeDeployPreprocessFunction {
 public:
  Resize(int width, int height);
  ~Resize() override;
  bool Run(BreezeDeployMat &tensor) override;

 private:
  int width_ = 0;
  int height_ = 0;
};
}
}
#endif //BREEZE_DEPLOY_PREPROCESS_FUNCTION_RESIZE_RESIZE_H_
