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

#ifndef BREEZE_DEPLOY_BACKENDS_ONNX_RUNTIME_ONNX_BACKEND_OPTION_H_
#define BREEZE_DEPLOY_BACKENDS_ONNX_RUNTIME_ONNX_BACKEND_OPTION_H_

#include "onnxruntime_cxx_api.h"

namespace breeze_deploy {
namespace backend {
class ONNXBackendOption {
 public:
  ONNXBackendOption() = default;
  ONNXBackendOption(const ONNXBackendOption &onnx_backend_option) {
    ort_logging_level_ = onnx_backend_option.ort_logging_level_;
    log_id_ = onnx_backend_option.log_id_;
    graph_optimization_level_ = onnx_backend_option.graph_optimization_level_;
    intra_op_num_threads_ = onnx_backend_option.intra_op_num_threads_;
    inter_op_num_threads_ = onnx_backend_option.inter_op_num_threads_;
  }
  ONNXBackendOption &operator=(const ONNXBackendOption &breeze_deploy_backend_option) {
    if (this != &breeze_deploy_backend_option) {
      ort_logging_level_ = breeze_deploy_backend_option.ort_logging_level_;
      log_id_ = breeze_deploy_backend_option.log_id_;
      graph_optimization_level_ = breeze_deploy_backend_option.graph_optimization_level_;
      intra_op_num_threads_ = breeze_deploy_backend_option.intra_op_num_threads_;
      inter_op_num_threads_ = breeze_deploy_backend_option.inter_op_num_threads_;
    }
    return *this;
  }

  OrtLoggingLevel GetOrtLoggingLevel() {
    return ort_logging_level_;
  }

  const std::string &GetLogID() {
    return log_id_;
  }

  GraphOptimizationLevel GetGraphOptimizationLevel() {
    return graph_optimization_level_;
  }

  int GetIntraOPNumThreads() const {
    return intra_op_num_threads_;
  }

  int GetInterOPNumThreads() const {
    return inter_op_num_threads_;
  }

 private:
  OrtLoggingLevel ort_logging_level_ = OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING;
  std::string log_id_ = "ONNXBackend";
  GraphOptimizationLevel graph_optimization_level_ = GraphOptimizationLevel::ORT_ENABLE_ALL;
  int intra_op_num_threads_ = -1;
  int inter_op_num_threads_ = -1;
};
}
}

#endif //BREEZE_DEPLOY_BACKENDS_ONNX_RUNTIME_ONNX_BACKEND_OPTION_H_
