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

#ifndef BREEZE_DEPLOY_BACKENDS_BREEZE_DEPLOY_BACKEND_OPTION_H_
#define BREEZE_DEPLOY_BACKENDS_BREEZE_DEPLOY_BACKEND_OPTION_H_

#include <string>
#include "breeze_deploy/backends/onnx_backend/onnx_backend_option.h"

namespace breeze_deploy {
namespace backend {
class BreezeDeployBackendOption {
 public:
  BreezeDeployBackendOption() = default;
  BreezeDeployBackendOption(const BreezeDeployBackendOption &c);
  BreezeDeployBackendOption &operator=(const BreezeDeployBackendOption &breeze_deploy_backend_option);
  ~BreezeDeployBackendOption() = default;
  const std::string &GetModelPath() const;
  void SetModelPath(const std::string &model_path);
  const ONNXBackendOption &GetONNXBackendOption();

 private:
  std::string model_path_;
  ONNXBackendOption onnx_backend_option_{};
};
}
}
#endif //BREEZE_DEPLOY_BACKENDS_BREEZE_DEPLOY_BACKEND_OPTION_H_
