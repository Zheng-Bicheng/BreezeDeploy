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

#include "breeze_deploy/models/classification/classification_model.h"
#include "breeze_deploy/models/detection/detection_model.h"

namespace breeze_deploy {
namespace models {
class ImageRecognition {
 public:
  ImageRecognition(std::unique_ptr<DetectionModel> detection_model,
				   std::unique_ptr<ClassificationModel> classification_model)
	  : detection_model_{std::move(detection_model)}, classification_model_{std::move(classification_model)} {
  }

  bool BuildDatabase(const std::string& image_folder_path, bool use_detection = true);

 private:
  std::unique_ptr<DetectionModel> detection_model_;
  std::unique_ptr<ClassificationModel> classification_model_;
};
}
}
#endif //BREEZE_DEPLOY_PIPELINE_IMAGE_RECOGNITION_IMAGE_RECOGNITION_H_
