// Copyright (c) 2024/1/30 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_PIPELINE_IMAGE_RECOGNITION_IMAGE_RECOGNITION_H_
#define BREEZE_DEPLOY_PIPELINE_IMAGE_RECOGNITION_IMAGE_RECOGNITION_H_

#include <utility>
#include "breeze_deploy/index/breeze_deploy_index.h"
#include "breeze_deploy/models/feature/feature_model.h"
#include "breeze_deploy/models/detection/detection_model.h"

namespace breeze_deploy {
namespace models {
using namespace breeze_deploy::index;
class BREEZE_DEPLOY_EXPORT ImageRecognition {
 public:
  explicit ImageRecognition(std::unique_ptr<FeatureModel> recognition_model,
							std::unique_ptr<DetectionModel> detection_model = nullptr);
  bool Initialize(const BreezeDeployBackendOption &rec_option = BreezeDeployBackendOption(),
				  const BreezeDeployBackendOption &det_option = BreezeDeployBackendOption());
  bool CreateIndex();
  bool DeleteIndex();
  bool AddToDatabase(const cv::Mat& input_mat, int64_t image_index, bool use_detection = true);
  bool Predict(const cv::Mat &image,
			   ImageRecognitionResult &image_recognition_result,
			   size_t k = 1,
			   bool use_detection = true);

 private:
  std::unique_ptr<DetectionModel> detection_model_ = nullptr;
  std::unique_ptr<FeatureModel> recognition_model_ = nullptr;
  std::unique_ptr<BreezeDeployIndex> index_system_ = nullptr;

  size_t feature_vector_length_ = 0;

  // 用于获取数据库文件夹
  bool DetectionPredict(const cv::Mat &input_image, DetectionResult &detection_result);
  std::vector<std::vector<float>> RecognitionPredict(const std::vector<cv::Mat> &input_image_vector);
};
}
}
#endif //BREEZE_DEPLOY_PIPELINE_IMAGE_RECOGNITION_IMAGE_RECOGNITION_H_
