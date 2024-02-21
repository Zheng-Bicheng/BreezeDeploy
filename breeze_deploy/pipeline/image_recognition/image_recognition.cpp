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
std::vector<std::string> ImageRecognition::GetDatabaseFolders(const std::string &database_path) {
  std::vector<std::string> database_folders;
  auto result_get = utils::filesystem::GetFolders(database_path, database_folders);
  if (!result_get) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to retrieve folders in the path. Please check if the path({}) is correct.",
							   database_path)
	return {};
  }
  return std::move(database_folders);
}
std::vector<std::string> ImageRecognition::GetDatabaseFiles(const std::string &database_folder_path) {
  std::vector<std::string> database_files;
  auto result_get = utils::filesystem::GetFiles(database_folder_path, database_files);
  if (!result_get) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to retrieve files in the path. Please check if the path({}) is correct.",
							   database_folder_path)
	return {};
  }
  return std::move(database_files);
}
std::vector<cv::Mat> ImageRecognition::DetectionPredict(const cv::Mat &input_image, DetectionResult &detection_result) {
  if (input_image.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("The input image is empty.")
	return {};
  }
  BREEZE_DEPLOY_LOGGER_ASSERT(detection_model_ != nullptr, "The detection model does not exist, "
														   "and the inference operation for the detection model cannot be executed.")
  if (!detection_model_->Predict(input_image, detection_result)) {
	BREEZE_DEPLOY_LOGGER_ERROR("During the prediction process, an error occurred in the detection model.")
	return {};
  }
  return utils::image_process::CropImage(input_image, detection_result);
}
std::vector<cv::Mat> ImageRecognition::DetectionPredict(const cv::Mat &input_image) {
  DetectionResult detection_result;
  return DetectionPredict(input_image, detection_result);
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
std::vector<std::vector<float>> ImageRecognition::GetFeature(const cv::Mat &input_image, bool use_detection) {
  std::vector<cv::Mat> rec_image_vector;
  rec_image_vector.emplace_back(input_image);

  // 判断是否需要使用检测模型
  if (detection_model_ != nullptr && use_detection) {
	rec_image_vector = DetectionPredict(input_image);
	if (rec_image_vector.empty()) {
	  BREEZE_DEPLOY_LOGGER_WARN("The detection model did not detect the target in the image. "
								"The original image will be used instead for feature extraction.")
	  rec_image_vector.emplace_back(input_image);
	}
  }
  return std::move(RecognitionPredict(rec_image_vector));
}
bool ImageRecognition::BuildDatabase(const std::string &database_path, bool use_detection) {
  // 初始化向量数据库
  index_system_ = std::make_unique<BreezeDeployIndex>(feature_vector_length_, "Flat");

  // 获取数据库路径下的所有文件夹
  std::vector<std::string> database_folders = GetDatabaseFolders(database_path);
  if (database_folders.empty()) {
	BREEZE_DEPLOY_LOGGER_WARN("No folders exist in the path. Please check if the path({}) is correct.", database_path)
	return false;
  }

  // 遍历数据库路径下的所有文件夹
  for (int folder_index = 0; folder_index < database_folders.size(); folder_index++) {
	auto &database_folder = database_folders[folder_index];
	// 获取数据库文件夹下的所有图片
	std::string database_folder_path = utils::filesystem::JoinPath({database_path, database_folder});
	std::vector<std::string> image_file_names = GetDatabaseFiles(database_folder_path);
	if (image_file_names.empty()) {
	  BREEZE_DEPLOY_LOGGER_WARN("No files exist in the path. Please check if the path({}) is correct.",
								database_folder_path)
	  continue;
	}

	// 遍历数据库文件夹下的所有图片
	for (int file_index = 0; file_index < image_file_names.size(); file_index++) {
	  auto &image_file_name = image_file_names[file_index];
	  std::string image_file_path = utils::filesystem::JoinPath({database_folder_path, image_file_name});
	  BREEZE_DEPLOY_LOGGER_DEBUG("Predict: [{}/{}] [{}/{}] [{}]",
								 folder_index,
								 database_folders.size(),
								 file_index,
								 image_file_names.size(),
								 image_file_path);

	  // 根据后缀，判断当前文件是否是图片
	  if (!utils::filesystem::IsImage(image_file_name)) {
		BREEZE_DEPLOY_LOGGER_WARN("Based on the image extension, this file({}) may not be an image. "
								  "Please modify it to an appropriate file extension.", image_file_name)
		continue;
	  }

	  // 推理图片获取图片特征
      auto infer_image = cv::imread(image_file_path);
	  auto feature_vector = GetFeature(infer_image, use_detection);
	  if (feature_vector.empty()) {
		BREEZE_DEPLOY_LOGGER_WARN("No target detected in the image.")
		continue;
	  }
	  if (feature_vector.size() > 1) {
		BREEZE_DEPLOY_LOGGER_WARN("Detecting multiple objects in an image.")
		continue;
	  }

	  // 将特征添加到特征搜索系统中
	  if (!index_system_->AddFeature(feature_vector[0], {folder_index})) {
		BREEZE_DEPLOY_LOGGER_ERROR("Failed to add feature.");
		continue;
	  }
	}
  }
  return true;
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
	rec_image_vector = DetectionPredict(image, detection_result);
  } else {
	rec_image_vector.emplace_back(image);
  }
  if (rec_image_vector.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("The array for recognizing images is empty, "
							   "possibly due to the detection model not producing output or an error in the input image path.")
	return false;
  }

  auto feature_vector = RecognitionPredict(rec_image_vector);
  if (feature_vector.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("The feature vector is empty")
	return false;
  }

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

