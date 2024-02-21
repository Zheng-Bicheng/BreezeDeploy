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
    rknn_destroy_mem(ctx_, input_mems_[i]);
  }

  for (uint32_t i = 0; i < io_num_.n_output; i++) {
    rknn_destroy_mem(ctx_, output_mems_[i]);
  }
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

  // Get RKNN QUERY INPUT OUTPUT NUM
  ret = rknn_query(ctx_, RKNN_QUERY_IN_OUT_NUM, &io_num_, sizeof(io_num_));
  if (ret != RKNN_SUCC) {
    BREEZE_DEPLOY_LOGGER_ERROR("The function(rknn_query RKNN_QUERY_IN_OUT_NUM) return failed! code is {}.", ret)
    return false;
  }

  // Allocate memory for private variable input_attrs_.
  input_attrs_ = (rknn_tensor_attr *) malloc(sizeof(rknn_tensor_attr) * io_num_.n_input);
  input_tensor_info_vector_.resize(io_num_.n_input);
  memset(input_attrs_, 0, io_num_.n_input * sizeof(rknn_tensor_attr));
  for (uint32_t i = 0; i < io_num_.n_input; i++) {
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

    // Copy input_attrs_ to input tensor info
    auto input_tensor_info = input_tensor_info_vector_[i];
    auto &temp_name = input_tensor_info.tensor_name;
    auto &temp_shape = input_tensor_info.tensor_shape;
    auto &temp_data_type = input_tensor_info.tensor_type;
    temp_name = std::string(input_attrs_[i].name);
    temp_shape.resize(input_attrs_[i].n_dims);
    for (int j = 0; j < input_attrs_[i].n_dims; j++) {
      temp_shape[j] = input_attrs_[i].dims[j];
    }
    temp_data_type = RknnTensorTypeToFDDataType(input_attrs_[i].type);
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

    // FastDeploy Only support postprocess when output type is fp32,
    // so output_attrs_.type needs to be fixed as RKNN_TENSOR_FLOAT32.
    output_attrs_[i].type = RKNN_TENSOR_FLOAT32;

    // Copy output_attrs_ to input tensor info
    auto input_tensor_info = output_tensor_info_vector_[i];
    auto &temp_name = input_tensor_info.tensor_name;
    auto &temp_shape = input_tensor_info.tensor_shape;
    auto &temp_data_type = input_tensor_info.tensor_type;
    temp_name = std::string(output_attrs_[i].name);
    temp_shape.resize(output_attrs_[i].n_dims);
    for (int j = 0; j < output_attrs_[i].n_dims; j++) {
      temp_shape[j] = output_attrs_[i].dims[j];
    }
    temp_data_type = RknnTensorTypeToFDDataType(output_attrs_[i].type);
  }
  return true;
}
bool RKNPUBackend::Infer(std::vector<BreezeDeployTensor> &input_tensor,
                         std::vector<BreezeDeployTensor> &output_tensor) {
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
      BREEZE_DEPLOY_LOGGER_ERROR("Input tensor({}) is nullptr.",i)
      return false;
    }
    memcpy(input_mems_[i]->virt_addr, input_tensor[i].GetConstTensorDataPointer(), input_tensor[i].GetTensorDataByteSize());
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
    auto tensor_data = reinterpret_cast<uint8_t *>(output_mems_[i]->virt_addr);
    auto &output_tensor_info = output_tensor_info_vector_[i];
    auto output_shape = output_tensor_info.tensor_shape;
    auto tensor_data_type = BreezeDeployTensorDataType::FP32;
    output_tensor[i].SetTensorData(tensor_data, output_shape, tensor_data_type);
  }
  return true;
}

BreezeDeployTensorDataType RKNPUBackend::RknnTensorTypeToFDDataType(rknn_tensor_type type) {
  if (type == rknn_tensor_type::RKNN_TENSOR_FLOAT16) {
    return BreezeDeployTensorDataType::FP32;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_FLOAT32) {
    return BreezeDeployTensorDataType::FP32;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_INT8) {
    return BreezeDeployTensorDataType::INT8;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_INT16) {
    return BreezeDeployTensorDataType::INT16;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_INT32) {
    return BreezeDeployTensorDataType::INT32;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_UINT8) {
    return BreezeDeployTensorDataType::UINT8;
  }
  if (type == rknn_tensor_type::RKNN_TENSOR_BOOL) {
    return BreezeDeployTensorDataType::BOOL;
  }
  BREEZE_DEPLOY_LOGGER_ERROR("BreezeDeployTensorDataType don't support this type.")
  return BreezeDeployTensorDataType::UNKNOWN;
}
}
}
#endif