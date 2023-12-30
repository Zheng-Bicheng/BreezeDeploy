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

#ifndef BREEZE_DEPLOY_POSTPROCESS_FUNCTION_TOPK_TOP_K_H_
#define BREEZE_DEPLOY_POSTPROCESS_FUNCTION_TOPK_TOP_K_H_
#include "breeze_deploy/postprocess_function/breeze_deploy_postprocess_function.h"
namespace breeze_deploy {
namespace function {
class TopK : public BreezeDeployPostprocessFunction {
 public:
  explicit TopK(size_t k);
  virtual bool Run(BreezeDeployTensor &tensor, std::vector<ClassificationResult> &result) override;
 private:
  size_t k_ = 1;
};
}
}
#endif //BREEZE_DEPLOY_POSTPROCESS_FUNCTION_TOPK_TOP_K_H_
