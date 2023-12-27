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

#ifndef BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_TIME_H_
#define BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_TIME_H_

#include <chrono>
#include <string>

namespace breeze_deploy {
class BreezeDeployTime {
 public:
  void Start();
  void End();
  void PrintInfo(const std::string &prefix, float proportion = 1.0);
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> begin_;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_;
};
}
#endif //BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_TIME_H_
