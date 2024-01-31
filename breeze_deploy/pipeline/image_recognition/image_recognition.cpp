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
#include <dirent.h>
#include "breeze_deploy/utils/utils.h"
#include "breeze_deploy/pipeline/image_recognition/image_recognition.h"
namespace breeze_deploy {
namespace models {
bool ImageRecognition::BuildDatabase(const std::string &image_folders_path, bool use_detection) {
  // 获取当前路径下的所有文件夹
  std::vector<std::string> image_folders;
  auto result_get = utils::GetSubdirectories(image_folders_path, image_folders);
  if (!result_get) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to get image folders.")
	return false;
  }
  if (image_folders.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("The folder({}) does not exist in the current path.", image_folders_path)
	return false;
  }

  // 获取当前文件夹下的所有图片
  for (auto &image_folder : image_folders) {
	BREEZE_DEPLOY_LOGGER_DEBUG("文件夹: {}", image_folder)
	std::string image_folder_path = image_folders_path;
	image_folder_path += "/";
	image_folder_path += image_folder;

	std::vector<std::string> image_files;
	result_get = utils::GetFiles(image_folder_path, image_files);
	if (!result_get) {
	  BREEZE_DEPLOY_LOGGER_ERROR("Failed to get image files.")
	  return false;
	}
	for (auto &image_file : image_files) {
	  BREEZE_DEPLOY_LOGGER_DEBUG("文件: {}", image_file)
	}
  }
  return true;
}
}
}

