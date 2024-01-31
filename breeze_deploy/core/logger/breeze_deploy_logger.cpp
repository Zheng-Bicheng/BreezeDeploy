// Copyright (c) 2023/12/28 Zheng-Bicheng. All Rights Reserved.
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
#include "breeze_deploy_logger.h"
namespace breeze_deploy {
BreezeDeployLogger::BreezeDeployLogger() {
  auto color_stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_st>();
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(color_stdout_sink);
  base_logger_ = std::make_shared<spdlog::logger>(logger_name_, begin(sinks), end(sinks));
  base_logger_->set_level(spdlog::level::debug);
  base_logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%s,%!,%#][%l]: %^%v%$");
}
BreezeDeployLogger::~BreezeDeployLogger() {
  spdlog::shutdown();
}
const std::shared_ptr<spdlog::logger> &BreezeDeployLogger::GetLogger() {
  return base_logger_;
}
BreezeDeployLogger breeze_deploy_logger;
}