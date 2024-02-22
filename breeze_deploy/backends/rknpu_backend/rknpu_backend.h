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

#ifdef ENABLE_RKNN_RUNTIME

#ifndef BREEZE_DEPLOY_RKNPU_BACKEND_H
#define BREEZE_DEPLOY_RKNPU_BACKEND_H

#include "breeze_deploy/backends/breeze_deploy_backend.h"
#include "rknn_api.h"

namespace breeze_deploy {
namespace backend {
class RKNPUBackend : public BreezeDeployBackend {
 public:
  RKNPUBackend() = default;
  ~RKNPUBackend() override;
  std::string BackendName() override { return "RKNPUBackend"; }
  bool Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) override;
  bool Infer(std::vector<BreezeDeployTensor> &input_tensor, std::vector<BreezeDeployTensor> &output_tensor) override;

 private:
  rknn_context ctx_{};

  rknn_sdk_version sdk_ver_{};

  rknn_input_output_num io_num_{0, 0};

  rknn_tensor_attr *input_attrs_ = nullptr;
  rknn_tensor_attr *output_attrs_ = nullptr;
  bool InitInputOutputAttributes();
  uint32_t GetAttributeByteSize(const rknn_tensor_attr &attr) const;
  void PrintAttribute(rknn_tensor_attr &attr);

  std::vector<rknn_tensor_mem *> input_memories_ = {};
  std::vector<rknn_tensor_mem *> output_memories_ = {};
  bool InitInputOutputMemories();

  static BreezeDeployTensorType RKNNTensorTypeToBDTensorType(rknn_tensor_type type);
  static rknn_tensor_type BDTensorDataTypeToRKNNTensorType(BreezeDeployTensorType type);
};
}
}

#endif //BREEZE_DEPLOY_RKNPU_BACKEND_H

#endif // ENABLE_RKNN_RUNTIME