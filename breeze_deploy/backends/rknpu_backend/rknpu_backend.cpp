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
#include "breeze_deploy/backends/rknpu_backend/rknpu_backend.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"

namespace breeze_deploy {
namespace backend {
RKNPUBackend::~RKNPUBackend() {
  if (input_attrs_ != nullptr) {
    free(input_attrs_);
  }

  if (output_attrs_ != nullptr) {
    free(output_attrs_);
  }

  for (uint32_t i = 0; i < io_num_.n_input; i++) {
    rknn_destroy_mem(ctx_, input_memories_[i]);
  }

  for (uint32_t i = 0; i < io_num_.n_output; i++) {
    rknn_destroy_mem(ctx_, output_memories_[i]);
  }
}

void RKNPUBackend::PrintAttribute(rknn_tensor_attr &attr) {
  BREEZE_DEPLOY_LOGGER_INFO(
      "name={}, n_dims={}, dims=[{}, {}, {}, {}], n_elems={}, size={}, fmt={}, type={}, qnt_type={}, zp={}, scale={}, pass_through={}",
      attr.name,
      attr.n_dims,
      attr.dims[0],
      attr.dims[1],
      attr.dims[2],
      attr.dims[3],
      attr.n_elems,
      attr.size,
      get_format_string(attr.fmt),
      get_type_string(attr.type),
      get_qnt_type_string(attr.qnt_type),
      attr.zp,
      attr.scale,
      attr.pass_through);
}

bool RKNPUBackend::InitInputOutputAttributes() {
  int ret = RKNN_SUCC;

  // Get RKNN QUERY INPUT OUTPUT NUM
  ret = rknn_query(ctx_, RKNN_QUERY_IN_OUT_NUM, &io_num_, sizeof(io_num_));
  if (ret != RKNN_SUCC) {
    BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_query RKNN_QUERY_IN_OUT_NUM) return failed! code is {}.", ret)
    return false;
  }

  // Allocate memory for private variable input_attrs_.
  auto input_tensor_number = io_num_.n_input;
  input_attrs_ = (rknn_tensor_attr *) malloc(sizeof(rknn_tensor_attr) * input_tensor_number);
  input_tensor_info_vector_.resize(input_tensor_number);
  memset(input_attrs_, 0, input_tensor_number * sizeof(rknn_tensor_attr));
  for (uint32_t i = 0; i < input_tensor_number; i++) {
    input_attrs_[i].index = i;

    ret = rknn_query(ctx_, RKNN_QUERY_INPUT_ATTR, &(input_attrs_[i]), sizeof(rknn_tensor_attr));
    if (ret != RKNN_SUCC) {
      BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_query RKNN_QUERY_INPUT_ATTR) return failed! code is {}.", ret)
      return false;
    }

    if ((input_attrs_[i].fmt != RKNN_TENSOR_NHWC) && (input_attrs_[i].fmt != RKNN_TENSOR_UNDEFINED)) {
      BREEZE_DEPLOY_LOGGER_ERROR("RKNPUBackend only support input format is NHWC or UNDEFINED.")
      return false;
    }

    PrintAttribute(input_attrs_[i]);

    // Copy input_attrs_ to input tensor info
    auto &input_tensor_info = input_tensor_info_vector_[i];
    auto &temp_name = input_tensor_info.tensor_name;
    auto &temp_shape = input_tensor_info.tensor_shape;
    auto &temp_data_type = input_tensor_info.tensor_type;
    temp_name = std::string(input_attrs_[i].name);
    temp_shape.resize(input_attrs_[i].n_dims);
    for (int j = 0; j < input_attrs_[i].n_dims; j++) {
      temp_shape[j] = input_attrs_[i].dims[j];
      if (temp_shape[j] == 0) {
        temp_shape[j] = 1;
      }
    }
    temp_data_type = RKNNTensorTypeToBDTensorType(input_attrs_[i].type);
  }

  // Allocate memory for private variable output_attrs_.
  output_attrs_ = (rknn_tensor_attr *) malloc(sizeof(rknn_tensor_attr) * io_num_.n_output);
  output_tensor_info_vector_.resize(io_num_.n_output);
  memset(output_attrs_, 0, io_num_.n_output * sizeof(rknn_tensor_attr));
  for (uint32_t i = 0; i < io_num_.n_output; i++) {
    output_attrs_[i].index = i;
    ret = rknn_query(ctx_, RKNN_QUERY_OUTPUT_ATTR, &(output_attrs_[i]), sizeof(rknn_tensor_attr));
    if (ret != RKNN_SUCC) {
      BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_query RKNN_QUERY_OUTPUT_ATTR) return failed! code is {}.", ret)
      return false;
    }

    PrintAttribute(output_attrs_[i]);

    // Copy output_attrs_ to input tensor info
    auto &input_tensor_info = output_tensor_info_vector_[i];
    auto &temp_name = input_tensor_info.tensor_name;
    auto &temp_shape = input_tensor_info.tensor_shape;
    auto &temp_data_type = input_tensor_info.tensor_type;
    temp_name = std::string(output_attrs_[i].name);
    temp_shape.resize(output_attrs_[i].n_dims);
    for (int j = 0; j < output_attrs_[i].n_dims; j++) {
      temp_shape[j] = output_attrs_[i].dims[j];
      if (temp_shape[j] == 0) {
        temp_shape[j] = 1;
      }
    }
    temp_data_type = RKNNTensorTypeToBDTensorType(output_attrs_[i].type);
  }
  return true;
}

uint32_t RKNPUBackend::GetAttributeByteSize(const rknn_tensor_attr &attr) const {
  auto data_type = RKNNTensorTypeToBDTensorType(attr.type);
  auto data_type_size = GetBDTensorTypeSize(data_type);
  return attr.n_elems * data_type_size;
}

bool RKNPUBackend::InitInputOutputMemories() {
  int ret = RKNN_SUCC;
  input_memories_.resize(io_num_.n_input);
  for (uint32_t i = 0; i < io_num_.n_input; i++) {
    input_memories_[i] = rknn_create_mem(ctx_, GetAttributeByteSize(input_attrs_[i]));
    if (input_memories_[i] == nullptr) {
      BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_create_mem) return failed! code is {}.", ret)
      return false;
    }

    // Set input tensor memory
    ret = rknn_set_io_mem(ctx_, input_memories_[i], &input_attrs_[i]);
    if (ret != RKNN_SUCC) {
      BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_set_io_mem) return failed! code is {}.", ret)
      return false;
    }
  }

  output_memories_.resize(io_num_.n_output);
  for (uint32_t i = 0; i < io_num_.n_output; ++i) {
    output_memories_[i] = rknn_create_mem(ctx_, GetAttributeByteSize(output_attrs_[i]));
    if (output_memories_[i] == nullptr) {
      BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_create_mem) return failed! code is {}.", ret)
      return false;
    }

    // Set output tensor memory
    ret = rknn_set_io_mem(ctx_, output_memories_[i], &output_attrs_[i]);
    if (ret != RKNN_SUCC) {
      BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_set_io_mem) return failed! code is {}.", ret)
      return false;
    }
  }
  return true;
}

bool RKNPUBackend::Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option) {
  int ret = RKNN_SUCC;

  // Init RKNN Runtime
  const auto &model_path = breeze_deploy_backend_option.GetModelPath();
  ret = rknn_init(&ctx_, (void *) model_path.data(), 0, 0, nullptr);
  if (ret != RKNN_SUCC) {
    BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_init) return failed! code is {}.", ret)
    return false;
  }

  // Get RKNN SDK VERSION
  ret = rknn_query(ctx_, RKNN_QUERY_SDK_VERSION, &sdk_ver_, sizeof(sdk_ver_));
  if (ret != RKNN_SUCC) {
    BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_query RKNN_QUERY_SDK_VERSION) return failed! code is {}.", ret)
    return false;
  }
  BREEZE_DEPLOY_LOGGER_INFO("rknpu2 driver version: {}", sdk_ver_.drv_version)
  BREEZE_DEPLOY_LOGGER_INFO("rknpu2 runtime version: {}", sdk_ver_.api_version)

  // Get input and output attributes
  if (!InitInputOutputAttributes()) {
    BREEZE_DEPLOY_LOGGER_ERROR("Failed to init input and output attributes.")
    return false;
  }
  return true;
}
bool RKNPUBackend::Infer(std::vector<BreezeDeployTensor> &input_tensor,
                         std::vector<BreezeDeployTensor> &output_tensor) {
  for (int i = 0; i < io_num_.n_input; ++i) {
    // Judge whether the input and output types are the same
    auto input_bd_tensor_type = input_tensor[i].GetTensorInfo().tensor_type;
    auto input_rknn_tensor_type = BDTensorDataTypeToRKNNTensorType(input_bd_tensor_type);
    if (input_rknn_tensor_type != input_attrs_[i].type) {
      auto model_rknn_tensor_type = input_attrs_[i].type;
      auto model_bd_tensor_type = RKNNTensorTypeToBDTensorType(model_rknn_tensor_type);
      input_attrs_[i].type = input_rknn_tensor_type;
      auto scale = GetBDTensorTypeSize(input_bd_tensor_type) / GetBDTensorTypeSize(model_bd_tensor_type);
      BREEZE_DEPLOY_LOGGER_WARN("The input tensor type != model's inputs type. "
                                "The input_type need {}, but inputs[{}].type is {}. "
                                "The input_size need *= scale({}).",
                                get_type_string(model_rknn_tensor_type),
                                i,
                                get_type_string(input_rknn_tensor_type),
                                scale)
      input_attrs_[i].size *= scale;
      input_attrs_[i].size_with_stride = input_attrs_[i].size;
    }
  }

  for (uint32_t i = 0; i < io_num_.n_output; i++) {
    auto output_bd_tensor_type = BreezeDeployTensorType::FP32;
    auto output_rknn_tensor_type = BDTensorDataTypeToRKNNTensorType(output_bd_tensor_type);
    output_attrs_[i].type = output_rknn_tensor_type;
    if (output_rknn_tensor_type != input_attrs_[i].type) {
      auto model_rknn_tensor_type = input_attrs_[i].type;
      auto model_bd_tensor_type = RKNNTensorTypeToBDTensorType(model_rknn_tensor_type);
      output_attrs_[i].type = output_rknn_tensor_type;
      auto scale = GetBDTensorTypeSize(output_bd_tensor_type) / GetBDTensorTypeSize(model_bd_tensor_type);
      BREEZE_DEPLOY_LOGGER_WARN("The output_tensor_type != model_output_type. "
                                "The output_tensor_type need {}, but model_output[{}].type is {}. "
                                "The model_output_size need *= scale({}).",
                                get_type_string(model_rknn_tensor_type),
                                i,
                                get_type_string(output_rknn_tensor_type),
                                scale)
      output_attrs_[i].size *= scale;
    }
  }

  if((input_memories_.size() != input_tensor.size()) || (output_memories_.size() != output_tensor.size())){
    if (!InitInputOutputMemories()) {
      BREEZE_DEPLOY_LOGGER_ERROR("Failed to init input and output memories.")
      return false;
    }
  }

  int ret = RKNN_SUCC;
  // Judge whether the input and output size are the same
  if (input_tensor.size() != input_tensor_info_vector_.size()) {
    BREEZE_DEPLOY_LOGGER_ERROR("Size of the inputs({}) should keep same with the inputs of this model({})",
                               input_tensor.size(), input_tensor_info_vector_.size())
    return false;
  }

  // Copy input data to input tensor memory
  for (uint32_t i = 0; i < io_num_.n_input; i++) {
    uint32_t width = input_attrs_[i].dims[2];
    uint32_t stride = input_attrs_[i].w_stride;
    if (width != stride) {
      BREEZE_DEPLOY_LOGGER_ERROR("Only support width == stride. But width is {}, stride is {}.", width, stride)
      return false;
    }

    if (input_tensor[i].GetConstTensorDataPointer() == nullptr) {
      BREEZE_DEPLOY_LOGGER_ERROR("Input tensor({}) is nullptr.", i)
      return false;
    }

    memcpy(input_memories_[i]->virt_addr,
           input_tensor[i].GetConstTensorDataPointer(),
           input_tensor[i].GetTensorDataByteSize());
  }

  // run rknn
  ret = rknn_run(ctx_, nullptr);
  if (ret != RKNN_SUCC) {
    BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_run) return failed! code is {}.", ret)
    return false;
  }

  // get result
  output_tensor.resize(output_tensor_info_vector_.size());
  for (int i = 0; i < output_tensor.size(); ++i) {
    auto tensor_data = reinterpret_cast<uint8_t *>(output_memories_[i]->virt_addr);
    auto &output_tensor_info = output_tensor_info_vector_[i];
    auto output_shape = output_tensor_info.tensor_shape;
    auto tensor_data_type = BreezeDeployTensorType::FP32;
    output_tensor[i].SetTensorData(tensor_data, output_shape, tensor_data_type);
  }
  return true;
}

BreezeDeployTensorType RKNPUBackend::RKNNTensorTypeToBDTensorType(rknn_tensor_type type) {
  if (type == rknn_tensor_type::RKNN_TENSOR_FLOAT16) {
    return BreezeDeployTensorType::FP16;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_FLOAT32) {
    return BreezeDeployTensorType::FP32;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_INT8) {
    return BreezeDeployTensorType::INT8;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_INT16) {
    return BreezeDeployTensorType::INT16;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_INT32) {
    return BreezeDeployTensorType::INT32;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_UINT8) {
    return BreezeDeployTensorType::UINT8;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_BOOL) {
    return BreezeDeployTensorType::BOOL;
  }
  BREEZE_DEPLOY_LOGGER_ERROR("BreezeDeployTensorDataType don't support this type.")
  return BreezeDeployTensorType::UNKNOWN;
}

rknn_tensor_type RKNPUBackend::BDTensorDataTypeToRKNNTensorType(BreezeDeployTensorType type) {
  if (type == BreezeDeployTensorType::FP16) {
    return rknn_tensor_type::RKNN_TENSOR_FLOAT16;
  }
  if (type == BreezeDeployTensorType::FP32) {
    return rknn_tensor_type::RKNN_TENSOR_FLOAT32;
  }
  if (type == BreezeDeployTensorType::INT8) {
    return rknn_tensor_type::RKNN_TENSOR_INT8;
  }
  if (type == BreezeDeployTensorType::INT16) {
    return rknn_tensor_type::RKNN_TENSOR_INT16;
  }
  if (type == BreezeDeployTensorType::INT32) {
    return rknn_tensor_type::RKNN_TENSOR_INT32;
  }
  if (type == BreezeDeployTensorType::UINT8) {
    return rknn_tensor_type::RKNN_TENSOR_UINT8;
  }
  if (type == BreezeDeployTensorType::BOOL) {
    return rknn_tensor_type::RKNN_TENSOR_BOOL;
  }
  BREEZE_DEPLOY_LOGGER_ERROR("BreezeDeployTensorDataType don't support this type.")
  return rknn_tensor_type::RKNN_TENSOR_TYPE_MAX;
}
}
}
#endif