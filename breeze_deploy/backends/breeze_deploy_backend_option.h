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
#include "breeze_deploy/backends/rknpu_backend/rknpu_backend_option.h"

namespace breeze_deploy {
namespace backend {

class BreezeDeployBackendOption {
 public:
  BreezeDeployBackendOption() = default;
  BreezeDeployBackendOption(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
    model_path_ = breeze_deploy_backend_option.model_path_;
    onnx_backend_option_ = breeze_deploy_backend_option.onnx_backend_option_;
    rknpu_backend_option_ = breeze_deploy_backend_option.rknpu_backend_option_;
  }
  BreezeDeployBackendOption &operator=(const BreezeDeployBackendOption &breeze_deploy_backend_option){
    if (this != &breeze_deploy_backend_option) {
      model_path_ = breeze_deploy_backend_option.model_path_;
      onnx_backend_option_ = breeze_deploy_backend_option.onnx_backend_option_;
      rknpu_backend_option_ = breeze_deploy_backend_option.rknpu_backend_option_;
    }
    return *this;
  }
  ~BreezeDeployBackendOption() = default;

  const std::string &GetModelPath() const{
    return model_path_;
  }
  void SetModelPath(const std::string &model_path) {
    model_path_ = model_path;
  }

  const ONNXBackendOption &GetONNXBackendOption(){
    return onnx_backend_option_;
  }
  const RKNPUBackendOption &GetRKNPUBackendOption(){
    return rknpu_backend_option_;
  }

 private:
  std::string model_path_;

  ONNXBackendOption onnx_backend_option_{};
  RKNPUBackendOption rknpu_backend_option_{};
};
}
}
#endif //BREEZE_DEPLOY_BACKENDS_BREEZE_DEPLOY_BACKEND_OPTION_H_
