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
#include "breeze_deploy/utils/utils.h"
#include "breeze_deploy/pipeline/image_recognition/image_recognition.h"
namespace breeze_deploy {
namespace models {
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
bool ImageRecognition::BuildDatabase(const std::string &database_path, bool use_detection) {
  // 获取当前路径下的所有文件夹
  std::vector<std::string> database_folders = GetDatabaseFolders(database_path);
  if (database_folders.empty()) {
	BREEZE_DEPLOY_LOGGER_WARN("No folders exist in the path. Please check if the path({}) is correct.", database_path)
	return false;
  }

  for (auto &database_folder : database_folders) {
	// 获取当前文件夹下的所有图片
	std::string database_folder_path = utils::filesystem::JoinPath({database_path, database_folder});
	std::vector<std::string> image_files = GetDatabaseFiles(database_folder_path);
	if (image_files.empty()) {
	  BREEZE_DEPLOY_LOGGER_WARN("No files exist in the path. Please check if the path({}) is correct.",
								database_folder_path)
	}
	for (auto &image_file : image_files) {

	  BREEZE_DEPLOY_LOGGER_DEBUG("文件: {}", image_file)
	}
  }
  return true;
}
bool ImageRecognition::Predict(const std::string &image_path, bool use_detection) {
  auto input_image = cv::imread(image_path);
  if (input_image.empty()) {
	BREEZE_DEPLOY_LOGGER_WARN("Failed to read image. Please check if the path({}) is correct.", image_path)
	return false;
  }

  std::vector<cv::Mat> rec_image_vector;
  if (use_detection) {

  } else {
	rec_image_vector.emplace_back(input_image);
  }

  return true;
}
}
}

