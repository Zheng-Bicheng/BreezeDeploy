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
#include <iostream>
#include "breeze_deploy/utils/utils.h"
#include <dirent.h>

namespace breeze_deploy{
namespace utils{
std::vector<std::string> GetSubdirectories(const std::string& path){
  std::vector<std::string> subdirectories;
  DIR* dir = opendir(path.c_str());
  if (dir != nullptr) {
	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
	  if (entry->d_type == DT_DIR && std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
		subdirectories.push_back(entry->d_name);
	  }
	}
	closedir(dir);
  } else {
	std::cerr << "Error opening directory " << path << std::endl;
  }

  return subdirectories;
}
}
}