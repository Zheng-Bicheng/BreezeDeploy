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

#include "breeze_deploy/pipeline/image_recognition/image_recognition.h"
namespace breeze_deploy{
namespace models{
bool ImageRecognition::BuildDatabase(const std::string &image_folder_path, bool use_detection) {
  // 获取当前路径下的所有文件夹名称
//  for (const auto& entry : std::file(currentPath)) {
//	// 检查是否是一个目录
//	if (fs::is_directory(entry.path())) {
//	  // 输出目录名称
//	  std::cout << "Folder: " << entry.path().filename() << std::endl;
//	}
//  }


  return false;
}
}
}

