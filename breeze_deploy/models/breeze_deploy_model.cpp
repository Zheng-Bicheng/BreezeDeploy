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

#include "breeze_deploy/models/breeze_deploy_model.h"
#include "breeze_deploy/backends/onnx_backend/onnx_backend.h"
namespace breeze_deploy {
namespace models {
bool BreezeDeployModel::Predict(const cv::Mat &input_mat) {
  return false;
}
bool BreezeDeployModel::Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  breeze_deploy_backend_ = std::make_shared<ONNXBackend>();
  auto result = breeze_deploy_backend_->Initialize(breeze_deploy_backend_option);
  return result;
}
}
}
