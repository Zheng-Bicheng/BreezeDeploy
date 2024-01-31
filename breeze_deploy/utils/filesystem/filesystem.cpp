// Copyright (c) 2024/1/31 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/utils/filesystem/filesystem.h"
#include <dirent.h>
namespace breeze_deploy {
namespace utils {
bool GetSubdirectories(const std::string &path, std::vector<std::string> &subdirectories) {
  subdirectories.clear();
  DIR *dir = opendir(path.c_str());
  if (dir == nullptr) {
	BREEZE_DEPLOY_LOGGER_ERROR("Error opening directory.")
	return false;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != nullptr) {
	if (entry->d_type != DT_DIR) {
	  continue;
	}
	if (std::string(entry->d_name) == ".") {
	  continue;
	}
	if (std::string(entry->d_name) == "..") {
	  continue;
	}
	subdirectories.emplace_back(entry->d_name);
  }
  closedir(dir);
  return true;
}

bool GetFiles(const std::string &path, std::vector<std::string> &files) {
  files.clear();
  DIR *dir = opendir(path.c_str());
  if (dir == nullptr) {
	BREEZE_DEPLOY_LOGGER_ERROR("Error opening directory.")
	return false;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != nullptr) {
	if (entry->d_type != DT_REG) {
	  continue;
	}
	if (std::string(entry->d_name) == ".") {
	  continue;
	}
	if (std::string(entry->d_name) == "..") {
	  continue;
	}
	files.emplace_back(entry->d_name);
  }
  closedir(dir);
  return true;
}
}
}