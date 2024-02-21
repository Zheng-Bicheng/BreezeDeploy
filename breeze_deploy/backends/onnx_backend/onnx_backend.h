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

#ifndef BREEZE_DEPLOY_BACKENDS_ONNXRUNTIME_ONNX_BACKEND_H_
#define BREEZE_DEPLOY_BACKENDS_ONNXRUNTIME_ONNX_BACKEND_H_

#include "onnxruntime_cxx_api.h"
#include "breeze_deploy/backends/breeze_deploy_backend.h"

namespace breeze_deploy {
namespace backend {
struct ONNXTensorInfo {
  std::string name;
  std::vector<int64_t> shape = {0, 0, 0, 0};
  ONNXTensorElementDataType type = ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
};

class ONNXBackend : public BreezeDeployBackend {
 public:
  ONNXBackend() = default;
  ~ONNXBackend() override = default;
  bool Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) override;
  bool Infer(std::vector<BreezeDeployTensor> &input_tensor, std::vector<BreezeDeployTensor> &output_tensor) override;

 private:
  Ort::Env env_{};
  Ort::SessionOptions session_options_{};
  Ort::Session session_{nullptr};

  std::vector<ONNXTensorInfo> onnx_input_tensor_info_vector_{};
  std::vector<const char *> input_node_vector_{};
  void SetInputTensorInfo();

  std::vector<ONNXTensorInfo> onnx_output_tensor_info_vector_{};
  std::vector<const char *> output_node_vector_{};
  void SetOutputTensorInfo();

  std::vector<Ort::Value> output_tensors_{};
};
}
}
#endif //BREEZE_DEPLOY_BACKENDS_ONNXRUNTIME_ONNX_BACKEND_H_
