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

#ifndef BREEZE_DEPLOY_BACKENDS_BREEZE_DEPLOY_BACKEND_H_
#define BREEZE_DEPLOY_BACKENDS_BREEZE_DEPLOY_BACKEND_H_

#include "breeze_deploy/core/tensor/breeze_deploy_tensor.h"
#include "breeze_deploy/backends/breeze_deploy_backend_option.h"

namespace breeze_deploy {
namespace backend {
class BreezeDeployBackend {
 public:
  BreezeDeployBackend() = default;
  virtual ~BreezeDeployBackend() = default;
  virtual bool Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) = 0;
  virtual bool Infer(std::vector<BreezeDeployTensor> &input_tensor, std::vector<BreezeDeployTensor> &output_tensor) = 0;
  virtual std::vector<BreezeDeployTensorInfo> GetInputTensorInfo() = 0;
  virtual std::vector<BreezeDeployTensorInfo> GetOutputTensorInfo() = 0;
  virtual size_t GetInputTensorSize() = 0;
  virtual size_t GetOutputTensorSize() = 0;

//  virtual bool PrintBackendTensorInfo();

 protected:
  BreezeDeployBackendOption breeze_deploy_backend_option_;
};
}
}

#endif //BREEZE_DEPLOY_BACKENDS_BREEZE_DEPLOY_BACKEND_H_
