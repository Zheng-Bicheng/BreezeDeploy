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

#include "breeze_deploy/core/breeze_deploy_logger.h"
#include "breeze_deploy/core/breeze_deploy_type.h"
namespace breeze_deploy {
size_t GetBreezeDeployDataTypeSize(BreezeDeployDataType breeze_deploy_data_type) {
  size_t size = 0;
  switch (breeze_deploy_data_type) {
	case BreezeDeployDataType::UINT8:
	  size = sizeof(uint8_t);
	  break;
	case BreezeDeployDataType::FP32:
	  size = sizeof(float);
	  break;
	case BreezeDeployDataType::UNKNOWN:
	  size = 0;
	  break;
  }
  return size;
}
}