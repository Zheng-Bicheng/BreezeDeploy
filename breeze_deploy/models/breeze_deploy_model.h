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

#ifndef BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_MODEL_H_
#define BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_MODEL_H_

#include <iostream>
#include <memory>
#include <yaml-cpp/yaml.h>

#include "breeze_deploy/backends/breeze_deploy_backend.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"
#include "breeze_deploy/core/mat/breeze_deploy_mat.h"
#include "breeze_deploy/core/tensor/breeze_deploy_tensor.h"
#include "breeze_deploy/core/time/breeze_deploy_time.h"
#include "breeze_deploy/models/preprocess/preprocess_function.h"

namespace breeze_deploy {
namespace models {
using namespace breeze_deploy::preprocess;
using namespace breeze_deploy::backend;
class BreezeDeployModel {
 public:
  BreezeDeployModel(const std::string &model_path, const std::string &config_file_path);
  virtual ~BreezeDeployModel() = default;
  bool Initialize(const BreezeDeployBackendOption &breeze_deploy_backend_option = BreezeDeployBackendOption());
  virtual std::string ModelName() { return "BreezeDeployModel"; }

 protected:
  // Model Attribute
  std::string model_path_;
  std::unique_ptr<BreezeDeployBackend> breeze_deploy_backend_ = nullptr;

  // Model Initialize
  std::string config_file_path_;
  bool ReadPreprocessYAML();
  virtual bool ReadPostprocessYAML() = 0;

  // Model Preprocess
  std::vector<std::shared_ptr<BreezeDeployPreprocess>> preprocess_functions_{};
  virtual bool Preprocess(const cv::Mat &input_mat) = 0;

  // Model Infer
  std::vector<BreezeDeployTensor> input_tensor_vector_{};
  std::vector<BreezeDeployTensor> output_tensor_vector_{};
  BreezeDeployBackendOption breeze_deploy_backend_option_;
  virtual bool Infer();

  // Model PostProcess
  virtual bool Postprocess() = 0;

  // Model Predict
  bool Predict(const cv::Mat &input_mat);
};
}
}
#endif //BREEZE_DEPLOY_MODELS_BREEZE_DEPLOY_MODEL_H_
