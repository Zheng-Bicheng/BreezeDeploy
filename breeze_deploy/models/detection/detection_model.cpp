// Copyright (c) 2024/1/22 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/detection/detection_model.h"

namespace breeze_deploy {
namespace models {
DetectionModel::DetectionModel(const std::string &model_path, const std::string &config_file_path)
	: BreezeDeployModel(model_path, config_file_path) {
}
bool DetectionModel::Infer() {
  return breeze_deploy_backend_->Infer(input_tensor_vector_, output_tensor_vector_);
}
}
}