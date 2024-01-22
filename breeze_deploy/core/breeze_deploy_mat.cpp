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
#include "breeze_deploy/core/breeze_deploy_mat.h"
#include "breeze_deploy/core/breeze_deploy_logger.h"
namespace breeze_deploy {
BreezeDeployMat::BreezeDeployMat(const cv::Mat &mat) {
  mat_ = mat;
}
cv::Mat &BreezeDeployMat::GetMat() {
  return mat_;
}
int BreezeDeployMat::GetWidth() const {
  return mat_.cols;
}
int BreezeDeployMat::GetHeight() const {
  return mat_.rows;
}
int BreezeDeployMat::GetChannel() const {
  return mat_.channels();
}
size_t BreezeDeployMat::GetMatDataByteSize() const {
  // Get the total number of data in the matrix.
  auto totalElements = mat_.total();
  // Get the number of bytes per element.
  auto elementSize = mat_.elemSize();
  // Calculate the total number of bytes.
  auto totalSize = totalElements * elementSize;
  return totalSize;
}
BreezeDeployTensorDataType BreezeDeployMat::GetMatDataType() {
  auto mat_data_type = mat_.type();
  switch (mat_data_type) {
	case CV_8UC3: return BreezeDeployTensorDataType::UINT8;
	case CV_32FC3: return BreezeDeployTensorDataType::FP32;
	default: BREEZE_DEPLOY_LOGGER_ERROR("BreezeDeployMat only support CV_8UC3/CV_32FC3, but type is {}.",
										cv::typeToString(mat_data_type))
	  return BreezeDeployTensorDataType::UNKNOWN;
  }
}
int BreezeDeployMat::GetMatType() {
  return mat_.type();
}

void BreezeDeployMat::SetDataFormat(BreezeDeployDataFormat format) {
  format_ = format;
}
BreezeDeployDataFormat BreezeDeployMat::GetMatDataFormat() const {
  return format_;
}
}
