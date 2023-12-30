// Copyright (c) 2023/12/30 Zheng-Bicheng. All Rights Reserved.
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

#ifndef BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_TYPE_H_
#define BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_TYPE_H_
#include <iostream>
namespace breeze_deploy {
enum class BreezeDeployDataType {
  UNKNOWN,
  UINT8,
  FP32
};
size_t GetBreezeDeployDataTypeSize(BreezeDeployDataType breeze_deploy_data_type);
}
#endif //BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_TYPE_H_