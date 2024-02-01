// Copyright (c) 2024/2/1 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_MODELS_FEATURE_FEATURE_MODEL_H_
#define BREEZE_DEPLOY_MODELS_FEATURE_FEATURE_MODEL_H_

#include "breeze_deploy/models/breeze_deploy_model.h"
#include "breeze_deploy/core/result/breeze_deploy_result.h"
namespace breeze_deploy {
namespace models {
class FeatureModel : public BreezeDeployModel {
 public:
  FeatureModel(const std::string &model_path, const std::string &config_file_path)
	  : BreezeDeployModel(model_path, config_file_path) {}
  std::string ModelName() override { return "FeatureModel"; }
  virtual bool Predict(const cv::Mat &input_mat, FeatureResult &label_result);
  virtual size_t GetFeatureLength();

 protected:
  // Model Initialize
  bool ReadPostprocessYAML() override;
  bool InitializeBackend(const BreezeDeployBackendOption &breeze_deploy_backend_option) override;


  bool Preprocess(const cv::Mat &input_mat) override;
  bool Postprocess() override;

  // For Softmax
  bool need_softmax_ = false;
};
}
}
#endif //BREEZE_DEPLOY_MODELS_FEATURE_FEATURE_MODEL_H_
