// Copyright (c) 2023/12/28 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_MODEL_H_
#define BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_MODEL_H_
#include <utility>
#include "breeze_deploy/models/breeze_deploy_model.h"
#include "breeze_deploy/models/classification/classification_result.h"
namespace breeze_deploy {
namespace models {
class ClassificationModel : public BreezeDeployModel {
 public:
  ClassificationModel(const std::string &model_path, const std::string &config_file_path)
	  : BreezeDeployModel(model_path, config_file_path) {}
  bool ReadLabelFile(const std::string &label_file_path);
  std::string ModelName() override { return "ClassificationModel"; }
  virtual bool Predict(const cv::Mat &input_mat, ClassificationLabelResult &label_result);
  virtual bool Predict(const cv::Mat &input_mat, ClassificationFeatureResult &label_result);
  static double CosineSimilarity(const ClassificationFeatureResult &a,
								const ClassificationFeatureResult &b);

 protected:
  bool Preprocess(const cv::Mat &input_mat) override;
  bool Postprocess() override;

  std::vector<std::string> labels_;

  bool ReadPostprocessYAML() override;
  // For Softmax
  bool need_softmax_ = false;
  // For TopK
  size_t k_ = 0;
  float min_confidence_ = 0;
  bool need_topk_ = false;
};
}
}
#endif //BREEZE_DEPLOY_MODELS_CLASSIFICATION_CLASSIFICATION_MODEL_H_
