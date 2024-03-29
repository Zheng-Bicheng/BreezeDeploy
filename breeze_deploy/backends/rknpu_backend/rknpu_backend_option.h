// Copyright (c) 2024/02/18 Zheng-Bicheng. All Rights Reserved.
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
#ifndef BREEZE_DEPLOY_BREEZE_DEPLOY_BACKENDS_RKNPU_BACKEND_RKNPU_BACKEND_OPTION_H_
#define BREEZE_DEPLOY_BREEZE_DEPLOY_BACKENDS_RKNPU_BACKEND_RKNPU_BACKEND_OPTION_H_

namespace breeze_deploy {
namespace backend {
class RKNPUBackendOption {
 public:
  RKNPUBackendOption() = default;
  RKNPUBackendOption(const RKNPUBackendOption &backend_option) {
    this->only_support_fp32_ = backend_option.only_support_fp32_;
  }
  RKNPUBackendOption &operator=(const RKNPUBackendOption &backend_option) {
    if (this != &backend_option) {
      this->only_support_fp32_ = backend_option.only_support_fp32_;
    }
    return *this;
  }

 private:
  bool only_support_fp32_ = true;
};
}
}
#endif //BREEZE_DEPLOY_BREEZE_DEPLOY_BACKENDS_RKNPU_BACKEND_RKNPU_BACKEND_OPTION_H_
