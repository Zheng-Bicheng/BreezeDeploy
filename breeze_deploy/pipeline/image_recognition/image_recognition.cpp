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

#include <filesystem>
#include "breeze_deploy/utils/filesystem/filesystem.h"
#include "breeze_deploy/pipeline/image_recognition/image_recognition.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"
#include "breeze_deploy/utils/image_process/crop/crop.h"
namespace breeze_deploy {
namespace models {
ImageRecognition::ImageRecognition(std::unique_ptr<FeatureModel> recognition_model,
                                   std::unique_ptr<DetectionModel> detection_model)
    : detection_model_{std::move(detection_model)}, recognition_model_{std::move(recognition_model)} {
  BREEZE_DEPLOY_LOGGER_ASSERT(recognition_model == nullptr,
                              "The feature extraction model must exist, but the current feature extraction model is empty.")
}

bool ImageRecognition::Initialize(const breeze_deploy::backend::BreezeDeployBackendOption &rec_option,
                                  const breeze_deploy::backend::BreezeDeployBackendOption &det_option) {
  if (!recognition_model_->Initialize(rec_option)) {
    BREEZE_DEPLOY_LOGGER_ERROR("Initialization of the recognition model failed. "
                               "Please check the configuration parameters of the recognition model.")
    return false;
  }
  feature_vector_length_ = recognition_model_->GetFeatureLength();

  if (detection_model_ != nullptr) {
    if (!detection_model_->Initialize(det_option)) {
      BREEZE_DEPLOY_LOGGER_ERROR("Initialization of the detection model failed. "
                                 "Please check the configuration parameters of the detection model.")
      return false;
    }
  }
  return true;
}

bool ImageRecognition::CreateIndex() {
  if (index_system_ != nullptr) {
    BREEZE_DEPLOY_LOGGER_ERROR(
        "The BreezeDeployIndex has already been created. Please use DeleteIndex() before using CreateIndex().")
    return false;
  }
  index_system_ = std::make_unique<BreezeDeployIndex>(feature_vector_length_, "Flat");
  return true;
}

bool ImageRecognition::DeleteIndex() {
  if (index_system_ == nullptr) {
    BREEZE_DEPLOY_LOGGER_ERROR(
        "BreezeDeployIndex has not been created yet. Please use CreateIndex() before using DeleteIndex().")
    return false;
  }
  index_system_.reset();
  return true;
}

bool ImageRecognition::DetectionPredict(const cv::Mat &input_image, DetectionResult &detection_result) {
  if (detection_model_ == nullptr) {
    BREEZE_DEPLOY_LOGGER_ERROR("The input image is empty.")
    return false;
  }

  if (input_image.empty()) {
    BREEZE_DEPLOY_LOGGER_ERROR("The input image is empty.")
    return false;
  }

  if (!detection_model_->Predict(input_image, detection_result)) {
    BREEZE_DEPLOY_LOGGER_ERROR("During the prediction process, an error occurred in the detection model.")
    return false;
  }
  return true;
}

bool ImageRecognition::AddToDatabase(const cv::Mat &input_mat, int64_t image_index, bool use_detection) {
  FeatureResult feature_result;
  if (use_detection) {
    DetectionResult detection_result;
    if (!DetectionPredict(input_mat, detection_result)) {
      BREEZE_DEPLOY_LOGGER_ERROR("During the prediction process, an error occurred in the detection model.")
      return false;
    }

    if (detection_result.Empty()) {
      BREEZE_DEPLOY_LOGGER_WARN("The detection model did not detect any images suitable for feature recognition.")
      return false;
    }

    auto max_result = detection_result.GetMaxConfidenceResult();
    auto infer_image = utils::image_process::CropImage(input_mat, max_result.rect_vector[0]);
    if (!recognition_model_->Predict(infer_image, feature_result)) {
      BREEZE_DEPLOY_LOGGER_ERROR("During the prediction process, an error occurred in the recognition model.")
      return false;
    }
  } else {
    if (!recognition_model_->Predict(input_mat, feature_result)) {
      BREEZE_DEPLOY_LOGGER_ERROR("During the prediction process, an error occurred in the recognition model.")
      return false;
    }
  }

  if (!index_system_->AddFeature(feature_result.feature_vector, {image_index})) {
    BREEZE_DEPLOY_LOGGER_ERROR("Adding features to the feature retrieval system failed.")
    return false;
  }
  return true;
}

std::vector<std::vector<float>> ImageRecognition::RecognitionPredict(const std::vector<cv::Mat> &input_image_vector) {
  std::vector<std::vector<float>> temp_feature_vector;
  // 遍历输入图片
  for (const auto &input_image : input_image_vector) {
    FeatureResult feature_result;
    if (!recognition_model_->Predict(input_image, feature_result)) {
      BREEZE_DEPLOY_LOGGER_ERROR("Image feature extraction model prediction failure.")
      continue;
    }
    temp_feature_vector.emplace_back(feature_result.feature_vector);
  }
  return std::move(temp_feature_vector);
}

bool ImageRecognition::Predict(const cv::Mat &image,
                               ImageRecognitionResult &image_recognition_result,
                               size_t k,
                               bool use_detection) {
  if (image.empty()) {
    BREEZE_DEPLOY_LOGGER_ERROR("The input image is empty.");
    return false;
  }

  std::vector<cv::Mat> rec_image_vector;
  auto &detection_result = image_recognition_result.detection_result;
  if (use_detection) {
    if (!DetectionPredict(image, detection_result)) {
      BREEZE_DEPLOY_LOGGER_ERROR("During the prediction process, an error occurred in the detection model.")
      return false;
    }

    if (detection_result.Empty()) {
      BREEZE_DEPLOY_LOGGER_WARN("The detection model did not detect any images suitable for feature recognition.")
      return true;
    }

    rec_image_vector = utils::image_process::CropImage(image, detection_result);
  } else {
    rec_image_vector.emplace_back(image);
  }

  auto feature_vector = RecognitionPredict(rec_image_vector);
  auto &classification_results = image_recognition_result.classification_results;
  classification_results.resize(feature_vector.size());
  for (int i = 0; i < feature_vector.size(); ++i) {
    auto &input_feature = feature_vector[i];
    auto &topk_confidence_vector = classification_results[i].topk_confidence_vector;
    auto &topk_label_id_vector = classification_results[i].topk_label_id_vector;
    index_system_->SearchIndex(input_feature,
                               k,
                               topk_confidence_vector,
                               topk_label_id_vector);
  }
  return true;
}
}
}

