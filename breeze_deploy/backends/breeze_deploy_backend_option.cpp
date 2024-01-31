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

#include "breeze_deploy/backends/breeze_deploy_backend_option.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"

namespace breeze_deploy {
namespace backend {
BreezeDeployBackendOption::BreezeDeployBackendOption(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  model_path_ = breeze_deploy_backend_option.model_path_;
  onnx_backend_option_ = breeze_deploy_backend_option.onnx_backend_option_;
}
BreezeDeployBackendOption &BreezeDeployBackendOption::operator=(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  if (this != &breeze_deploy_backend_option) {
	model_path_ = breeze_deploy_backend_option.model_path_;
	onnx_backend_option_ = breeze_deploy_backend_option.onnx_backend_option_;
  }
  return *this;
}
const std::string &BreezeDeployBackendOption::GetModelPath() const {
  return model_path_;
}
void BreezeDeployBackendOption::SetModelPath(const std::string &model_path) {
  model_path_ = model_path;
}
const ONNXBackendOption &BreezeDeployBackendOption::GetONNXBackendOption() {
  return onnx_backend_option_;
}
}
}