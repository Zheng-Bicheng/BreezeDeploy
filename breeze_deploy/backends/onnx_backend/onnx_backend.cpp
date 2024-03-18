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
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"
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

  // 设置输入Tensor
  SetInputTensorInfo();

  // 获取输出Tensor信息
  SetOutputTensorInfo();
  return true;
}

bool ONNXBackend::Infer(std::vector<BreezeDeployTensor> &input_tensor, std::vector<BreezeDeployTensor> &output_tensor) {
  Ort::MemoryInfo memory_info("Cpu", OrtDeviceAllocator, 0, OrtMemTypeDefault);
  std::vector<Ort::Value> input_tensors{};
  for (int i = 0; i < input_tensor.size(); ++i) {
	auto p_data = reinterpret_cast<float *>(input_tensor[i].GetTensorDataPointer());
	auto p_data_element_count = input_tensor[i].GetTensorSize();
	auto shape = input_tensor[i].GetTensorInfo().tensor_shape.data();
	auto shape_len = onnx_input_tensor_info_[i].shape.size();
	input_tensors.emplace_back(Ort::Value::CreateTensor<float>(
		memory_info,
		p_data,
		p_data_element_count,
		shape,
		shape_len));
  }

  auto run_options = Ort::RunOptions{nullptr};
  auto input_names = onnx_input_nodes_.data();
  auto input_values = input_tensors.data();
  auto input_count = onnx_input_nodes_.size();
  auto output_names = onnx_output_nodes_.data();
  auto output_count = onnx_output_nodes_.size();
  output_tensors_ = session_.Run(run_options, input_names, input_values, input_count, output_names, output_count);

  output_tensor.resize(output_tensors_.size());
  for (int i = 0; i < output_tensor.size(); ++i) {
	auto tensor_data = reinterpret_cast<uint8_t *>(output_tensors_[i].GetTensorMutableData<float>());
	auto tensor_type_and_shape_info = output_tensors_[i].GetTensorTypeAndShapeInfo();
	auto output_shape = tensor_type_and_shape_info.GetShape();
	auto tensor_data_type = BreezeDeployTensorType::FP32;
	output_tensor[i].SetTensorData(tensor_data, output_shape, tensor_data_type);
  }
  return true;
}
void ONNXBackend::SetInputTensorInfo() {
  Ort::AllocatorWithDefaultOptions allocator;
  // 获取输入Tensor个数
  size_t num_input_nodes = session_.GetInputCount();
  onnx_input_tensor_info_.resize(num_input_nodes);
  input_tensor_info_vector_.resize(num_input_nodes);

  // 设置输入Tensor参数
  for (int i = 0; i < num_input_nodes; ++i) {
	auto &input_tensor_info = onnx_input_tensor_info_[i];

	// copy tensor name
	auto input_tensor_node_name = session_.GetInputNameAllocated(i, allocator);
	input_tensor_info.name = input_tensor_node_name.get();

	// copy tensor type info
	auto input_type_info = session_.GetInputTypeInfo(i);
	auto tensor_type_and_shape_info = input_type_info.GetTensorTypeAndShapeInfo();
	input_tensor_info.type = tensor_type_and_shape_info.GetElementType();
	input_tensor_info.shape = tensor_type_and_shape_info.GetShape();

    input_tensor_info_vector_[i].tensor_shape = onnx_input_tensor_info_[i].shape;
    input_tensor_info_vector_[i].tensor_name = onnx_input_tensor_info_[i].name;
  }

  // 设置输入Tensor名称列表
  onnx_input_nodes_.resize(num_input_nodes);
  for (int i = 0; i < num_input_nodes; ++i) {
	auto &input_node_name = onnx_input_nodes_[i];
	auto &input_tensor_info = onnx_input_tensor_info_[i];
	input_node_name = input_tensor_info.name.c_str();
  }
}

void ONNXBackend::SetOutputTensorInfo() {
  Ort::AllocatorWithDefaultOptions allocator;
  // 获取输出Tensor个数
  size_t num_output_nodes = session_.GetOutputCount();
  onnx_output_tensor_info_.resize(num_output_nodes);

  // 获取输出Tensor参数
  for (int i = 0; i < num_output_nodes; ++i) {
	auto &output_tensor_info = onnx_output_tensor_info_[i];

	// copy tensor name
	auto output_tensor_node_name = session_.GetOutputNameAllocated(i, allocator);
	output_tensor_info.name = output_tensor_node_name.get();

	// copy tensor type info
	auto output_type_info = session_.GetOutputTypeInfo(i);
	auto tensor_type_and_shape_info = output_type_info.GetTensorTypeAndShapeInfo();
	output_tensor_info.type = tensor_type_and_shape_info.GetElementType();
	output_tensor_info.shape = tensor_type_and_shape_info.GetShape();
  }

  // 设置输出Tensor参数
  onnx_output_nodes_.resize(num_output_nodes);
  for (int i = 0; i < num_output_nodes; ++i) {
	auto &output_node_name = onnx_output_nodes_[i];
	auto &output_tensor_info = onnx_output_tensor_info_[i];
	output_node_name = output_tensor_info.name.c_str();
  }

  output_tensor_info_vector_.resize(num_output_nodes);
  for (int i = 0; i < num_output_nodes; ++i) {
    auto &output_tensor_info = onnx_output_tensor_info_[i];
    output_tensor_info_vector_[i].tensor_shape = output_tensor_info.shape;
    output_tensor_info_vector_[i].tensor_name = output_tensor_info.name;
    output_tensor_info_vector_[i].tensor_type = ONNXTypeToBDType(output_tensor_info.type);
  }
}

BreezeDeployTensorType ONNXBackend::ONNXTypeToBDType(ONNXTensorElementDataType type) {
  if (type == ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16) {
    return BreezeDeployTensorType::FP16;
  }
  if (type == ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
    return BreezeDeployTensorType::FP32;
  }
  if (type == ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8) {
    return BreezeDeployTensorType::INT8;
  }
  if (type == ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16) {
    return BreezeDeployTensorType::INT16;
  }
  if (type == ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32) {
    return BreezeDeployTensorType::INT32;
  }
  if (type == ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8) {
    return BreezeDeployTensorType::UINT8;
  }
  if (type == ONNXTensorElementDataType::ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL) {
    return BreezeDeployTensorType::BOOL;
  }
  BDLOGGER_ERROR("BreezeDeployTensorType don't support this type.")
  return BreezeDeployTensorType::UNKNOWN;
}
}
}