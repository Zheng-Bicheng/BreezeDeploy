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

#ifndef BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_LOGGER_H_
#define BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_LOGGER_H_

#define FMT_HEADER_ONLY
#define SPDLOG_HEADER_ONLY
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LOGGER_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"

namespace breeze_deploy {

#define BREEZE_DEPLOY_EXPORT __attribute__ ((visibility ("default")))

class BREEZE_DEPLOY_EXPORT BreezeDeployLogger {
 public:
  BreezeDeployLogger();
  ~BreezeDeployLogger();
  const std::shared_ptr<spdlog::logger> &GetLogger();

 private:
  std::string logger_name_ = "BreezeDeploy";
  std::shared_ptr<spdlog::logger> base_logger_;
};

BREEZE_DEPLOY_EXPORT extern BreezeDeployLogger breeze_deploy_logger;

#define BREEZE_DEPLOY_LOGGER_INFO(...)                     	\
{                                                         	\
  auto get_logger = breeze_deploy_logger.GetLogger(); 		\
  SPDLOG_LOGGER_INFO(get_logger, __VA_ARGS__);            	\
}

#define BREEZE_DEPLOY_LOGGER_DEBUG(...)                     \
{                                                         	\
  auto get_logger = breeze_deploy_logger.GetLogger(); 		\
  SPDLOG_LOGGER_DEBUG(get_logger, __VA_ARGS__);           	\
}

#define BREEZE_DEPLOY_LOGGER_WARN(...)                      \
{                                                         	\
  auto get_logger = breeze_deploy_logger.GetLogger(); 		\
  SPDLOG_LOGGER_WARN(get_logger, __VA_ARGS__);            	\
}

#define BDLOGGER_ERROR(...)                     \
{                                                         	\
  auto get_logger = breeze_deploy_logger.GetLogger(); 		\
  SPDLOG_LOGGER_ERROR(get_logger, __VA_ARGS__);           	\
}

#define BREEZE_DEPLOY_LOGGER_CRITICAL(...)               	\
{                                                         	\
  auto get_logger = breeze_deploy_logger.GetLogger(); 		\
  SPDLOG_LOGGER_CRITICAL(get_logger, __VA_ARGS__);        	\
}

#ifdef NDEBUG
#define BDLOGGER_ASSERT(flag, ...) 		\
{                                             		\
  if(!(flag)) {                                   	\
    BREEZE_DEPLOY_LOGGER_CRITICAL(__VA_ARGS__)   	\
	exit(-1);										\
  }                                            		\
}
#else
#define BREEZE_DEPLOY_LOGGER_ASSERT(flag, ...) 		\
{                                             		\
  if(!(flag)) {                                   	\
    BREEZE_DEPLOY_LOGGER_CRITICAL(__VA_ARGS__)   	\
  }                                            		\
  assert(flag);                                  	\
}
#endif

}
#endif //BREEZE_DEPLOY_CORE_BREEZE_DEPLOY_LOGGER_H_
