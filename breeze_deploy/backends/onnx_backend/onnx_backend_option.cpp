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

#include "breeze_deploy/backends/onnx_backend/onnx_backend_option.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"
namespace breeze_deploy {
namespace backend {
ONNXBackendOption::ONNXBackendOption(const ONNXBackendOption &onnx_backend_option) {
  ort_logging_level_ = onnx_backend_option.ort_logging_level_;
  log_id_ = onnx_backend_option.log_id_;
  graph_optimization_level_ = onnx_backend_option.graph_optimization_level_;
  intra_op_num_threads_ = onnx_backend_option.intra_op_num_threads_;
  inter_op_num_threads_ = onnx_backend_option.inter_op_num_threads_;
}
ONNXBackendOption &ONNXBackendOption::operator=(const ONNXBackendOption &breeze_deploy_backend_option) {
  if (this != &breeze_deploy_backend_option) {
	ort_logging_level_ = breeze_deploy_backend_option.ort_logging_level_;
	log_id_ = breeze_deploy_backend_option.log_id_;
	graph_optimization_level_ = breeze_deploy_backend_option.graph_optimization_level_;
	intra_op_num_threads_ = breeze_deploy_backend_option.intra_op_num_threads_;
	inter_op_num_threads_ = breeze_deploy_backend_option.inter_op_num_threads_;
  }
  return *this;
}
GraphOptimizationLevel ONNXBackendOption::GetGraphOptimizationLevel() {
  return graph_optimization_level_;
}
OrtLoggingLevel ONNXBackendOption::GetOrtLoggingLevel() {
  return ort_logging_level_;
}
int ONNXBackendOption::GetIntraOPNumThreads() const {
  return intra_op_num_threads_;
}
int ONNXBackendOption::GetInterOPNumThreads() const {
  return inter_op_num_threads_;
}
const std::string &ONNXBackendOption::GetLogID() {
  return log_id_;
}
}
}