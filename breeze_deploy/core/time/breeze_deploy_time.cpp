// Copyright (c) 2023/12/27 Zheng-Bicheng. All Rights Reserved.
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
#include "breeze_deploy_time.h"
namespace breeze_deploy {

void BreezeDeployTime::Start() {
  begin_ = std::chrono::high_resolution_clock::now();
}
void BreezeDeployTime::End() {
  end_ = std::chrono::high_resolution_clock::now();
}
float BreezeDeployTime::GetTimeDifference(const BreezeDeployTimeType &breeze_deploy_time_type) {
  float scale = 1;
  switch (breeze_deploy_time_type) {
	case BreezeDeployTimeType::Nanoseconds: scale = 1;
	  break;
	case BreezeDeployTimeType::Microseconds: scale = 1e-3;
	  break;
	case BreezeDeployTimeType::Milliseconds: scale = 1e-6;
	  break;
	case BreezeDeployTimeType::Seconds: scale = 1e-9;
	  break;
  }
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_ - begin_);
  auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_ - begin_);
  return static_cast<float>(elapsed.count()) * scale;
}
void BreezeDeployTime::PrintInfo(const std::string &prefix,
								 float proportion,
								 const BreezeDeployTimeType &breeze_deploy_time_type) {
  auto time_difference = GetTimeDifference(breeze_deploy_time_type);
  std::cout << prefix << " duration = " << time_difference * proportion << "." << std::endl;
}
}