// Copyright (c) 2024/3/28 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/segmentation/segmentation_model.h"

namespace breeze_deploy {
namespace models {
bool SegmentationModel::Preprocess(const cv::Mat &input_mat) {
  if (input_mat.empty()) {
    BDLOGGER_ERROR("input_mat is empty.")
    return false;
  }

  // Do preprocess
  input_mat_ = BreezeDeployMat(input_mat);
  for (const auto &preprocess_function : preprocess_functions_) {
    if (!preprocess_function->Run(input_mat_)) {
      BDLOGGER_ERROR("Failed to run preprocess.")
      return false;
    }
  }

  // Set data to tensor
  auto tensor_data = input_mat_.GetMat().data;
  auto tensor_data_type = input_mat_.GetMatDataType();
  auto c = input_mat_.GetChannel();
  auto h = input_mat_.GetHeight();
  auto w = input_mat_.GetWidth();
  if (input_mat_.GetMatDataFormat() == BreezeDeployMatFormat::CHW) {
    input_tensor_vector_[0].SetTensorData(tensor_data, {1, c, h, w}, tensor_data_type);
  } else {
    input_tensor_vector_[0].SetTensorData(tensor_data, {1, h, w, c}, tensor_data_type);
  }

  // Get resize radio and pad height/width.
  for (const auto &preprocess_function : preprocess_functions_) {
    if (preprocess_function->FunctionName() == "LetterBox" || preprocess_function->FunctionName() == "Resize") {
      pad_width_height_ = preprocess_function->GetPadWH();
      radio_width_height_ = preprocess_function->GetRadioWH();
    }
  }
  return true;
}

bool SegmentationModel::ProcessWithArgmax(SegmentationResult &segmentation_result) {
  auto &output_tensor = output_tensor_vector_[0];
  auto &output_tensor_shape = output_tensor.GetTensorInfo().tensor_shape;

  auto infer_height = output_tensor_shape[1];
  auto infer_width = output_tensor_shape[2];
  auto infer_chw = infer_height * infer_width;
  segmentation_result.Reserve(infer_chw);

  auto output_tensor_data = output_tensor.GetConstTensorDataPointer();
  auto &output_tensor_type = output_tensor.GetTensorInfo().tensor_type;
  switch (output_tensor_type) {
    case BDTensorType::INT64: {
      auto *infer_result_buffer = reinterpret_cast<const int64_t *>(output_tensor_data);
      for (int i = 0; i < infer_chw; i++) {
        segmentation_result.label_map[i] = static_cast<uint8_t>(*(infer_result_buffer + i));
      }
      break;
    }
    case BDTensorType::INT32: {
      auto *infer_result_buffer = reinterpret_cast<const int32_t *>(output_tensor_data);
      for (int i = 0; i < infer_chw; i++) {
        segmentation_result.label_map[i] = static_cast<uint8_t>(*(infer_result_buffer + i));
      }
      break;
    }
    case BDTensorType::UINT8: {
      auto *infer_result_buffer = reinterpret_cast<const uint8_t *>(output_tensor_data);
      for (int i = 0; i < infer_chw; i++) {
        segmentation_result.label_map[i] = *(infer_result_buffer + i);
      }
      break;
    }
    default: {
      BDLOGGER_ASSERT((false), "Output tensor type only support INT64/INT32/UINT8, But receive {}.", (int)output_tensor_type)
    }
  }
  return true;
}
bool SegmentationModel::ReadPostprocessYAML() {
  return true;
}
bool SegmentationModel::Postprocess() {
  return true;
}
bool SegmentationModel::Predict(const cv::Mat &input_mat,
                                SegmentationResult &segmentation_result) {
  BDModel::Predict(input_mat);
  auto vector_size = output_tensor_vector_.size();
  BDLOGGER_ASSERT((vector_size == 1), "SegmentationModel need vector size is 1.")

  return ProcessWithArgmax(segmentation_result);
}
cv::Mat SegmentationModel::Draw(const cv::Mat &mat, const SegmentationResult &segmentation_result) {
  return mat;
}
}
}