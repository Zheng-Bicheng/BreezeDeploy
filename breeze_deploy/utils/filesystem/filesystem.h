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

#ifndef BREEZE_DEPLOY_UTILS_FILESYSTEM_FILESYSTEM_H_
#define BREEZE_DEPLOY_UTILS_FILESYSTEM_FILESYSTEM_H_

#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace breeze_deploy {
namespace utils {
namespace filesystem{
bool IsImage(const std::string& file_path);
std::string JoinPath(const std::vector<std::string>& paths);
bool GetFolders(const std::string& path, std::vector<std::string>& subdirectories);
bool GetFiles(const std::string& path, std::vector<std::string>& files);
}
}
}
#endif //BREEZE_DEPLOY_UTILS_FILESYSTEM_FILESYSTEM_H_
