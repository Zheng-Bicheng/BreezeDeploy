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

#include "breeze_deploy/backends/onnx_backend/onnx_backend.h"
namespace breeze_deploy {
namespace backend {
bool ONNXBackend::Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  breeze_deploy_backend_option_ = breeze_deploy_backend_option;

  // Set Ort SessionOptions
  auto onnx_backend_option = breeze_deploy_backend_option_.GetONNXBackendOption();
  auto logging_level = onnx_backend_option.GetOrtLoggingLevel();
  auto log_id = onnx_backend_option.GetLogID().data();
  env_ = Ort::Env(logging_level, log_id);
  auto graph_optimization_level = onnx_backend_option.GetGraphOptimizationLevel();
  session_options_.SetGraphOptimizationLevel(graph_optimization_level);
  auto intra_op_num_threads = onnx_backend_option.GetIntraOPNumThreads();
  if (intra_op_num_threads > 0) {
	session_options_.SetIntraOpNumThreads(intra_op_num_threads);
  }
  auto inter_op_num_threads = onnx_backend_option.GetInterOPNumThreads();
  if (inter_op_num_threads > 0) {
	session_options_.SetInterOpNumThreads(inter_op_num_threads);
  }

  // Create Ort Session
  auto model_path = breeze_deploy_backend_option_.GetModelPath().data();
  session_ = Ort::Session(env_, model_path, session_options_);

  Ort::AllocatorWithDefaultOptions allocator;
  // 获取输入Tensor信息
  size_t num_input_nodes = session_.GetInputCount();
  input_tensor_info_vector_.resize(num_input_nodes);
  input_node_vector_.resize(num_input_nodes);
  for (int i = 0; i < num_input_nodes; ++i) {
	auto &input_tensor_info = input_tensor_info_vector_[i];
	auto &input_node_name = input_node_vector_[i];

	// copy tensor name
	auto input_tensor_node_name = session_.GetInputNameAllocated(i, allocator);
	input_tensor_info.name = input_tensor_node_name.get();
	input_node_name = input_tensor_info.name.c_str();

	// copy tensor type info
	auto input_type_info = session_.GetInputTypeInfo(i);
	auto tensor_type_and_shape_info = input_type_info.GetTensorTypeAndShapeInfo();
	input_tensor_info.type = tensor_type_and_shape_info.GetElementType();
	input_tensor_info.shape = tensor_type_and_shape_info.GetShape();
	for (auto &input_tensor : input_tensor_info.shape) {
	  if (input_tensor == -1) {
		input_tensor = 1;
	  }
	}
  }

  // 获取输出Tensor信息
  size_t num_output_nodes = session_.GetOutputCount();
  output_tensor_info_vector_.resize(num_output_nodes);
  output_node_vector_.resize(num_output_nodes);
  for (int i = 0; i < num_output_nodes; ++i) {
	auto &output_tensor_info = output_tensor_info_vector_[i];
	auto &output_node_name = output_node_vector_[i];

	// copy tensor name
	auto output_tensor_node_name = session_.GetOutputNameAllocated(i, allocator);
	output_tensor_info.name = output_tensor_node_name.get();
	output_node_name = output_tensor_info.name.c_str();

	// copy tensor type info
	auto output_type_info = session_.GetOutputTypeInfo(i);
	auto tensor_type_and_shape_info = output_type_info.GetTensorTypeAndShapeInfo();
	output_tensor_info.type = tensor_type_and_shape_info.GetElementType();
	output_tensor_info.shape = tensor_type_and_shape_info.GetShape();
	for (auto &output_tensor : output_tensor_info.shape) {
	  if (output_tensor == -1) {
		output_tensor = 1;
	  }
	}
  }

  return true;
}
bool ONNXBackend::Infer(std::vector<BreezeDeployTensor> &input_tensor, std::vector<BreezeDeployTensor> &output_tensor) {
  auto memory_info = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
  std::vector<Ort::Value> input_tensors{};
  for (int i = 0; i < input_tensor.size(); ++i) {
	auto p_data = reinterpret_cast<float *>(input_tensor[i].GetTensorData());
	auto p_data_element_count = input_tensor[i].GetTensorSize() / sizeof(float);
	auto shape = input_tensor_info_vector_[i].shape.data();
	auto shape_len = input_tensor_info_vector_[i].shape.size();
	input_tensors.emplace_back(Ort::Value::CreateTensor<float>(
		memory_info,
		p_data,
		p_data_element_count,
		shape,
		shape_len));
  }

  auto run_options = Ort::RunOptions{nullptr};
  auto input_names = input_node_vector_.data();
  auto input_values = input_tensors.data();
  auto input_count = input_node_vector_.size();
  auto output_names = output_node_vector_.data();
  auto output_count = output_node_vector_.size();
  output_tensors_ = session_.Run(run_options, input_names, input_values, input_count, output_names, output_count);

  output_tensor.resize(output_tensors_.size());
  for (int i = 0; i < output_tensor.size(); ++i) {
	auto tensor_data = reinterpret_cast<uint8_t *>(output_tensors_[i].GetTensorMutableData<float>());
	size_t tensor_size = 1;

	auto &output_tensor_info = output_tensor_info_vector_[i];
	for (size_t shape : output_tensor_info.shape) {
	  tensor_size *= shape;
	}
	tensor_size *= sizeof(float);
	output_tensor[i].SetTensorData(tensor_data, tensor_size);
  }
  return true;
}
}
}