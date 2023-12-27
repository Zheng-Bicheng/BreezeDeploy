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

#include "onnxruntime/onnxruntime_cxx_api.h"

namespace breeze_deploy {
namespace backend {
class ONNXBackendOption {
 public:
  OrtLoggingLevel GetOrtLoggingLevel();
  const std::string& GetLogID();
  GraphOptimizationLevel GetGraphOptimizationLevel();
  int GetIntraOPNumThreads();
  int GetInterOPNumThreads();

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
