// Copyright (c) 2024/1/22 Zheng-Bicheng. All Rights Reserved.
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

#include "breeze_deploy/models/detection/detection_model.h"

namespace breeze_deploy {
namespace models {
bool DetectionModel::Infer() {
  return breeze_deploy_backend_->Infer(input_tensor_vector_, output_tensor_vector_);
}
bool DetectionModel::ReadPostprocessYAML() {
  YAML::Node yaml_config;
  try {
	yaml_config = YAML::LoadFile(config_file_path_);
  } catch (YAML::BadFile &e) {
	BREEZE_DEPLOY_LOGGER_ERROR("Failed to load yaml file: {}.", config_file_path_)
	return false;
  }

  // Get postprocess root node
  auto postprocess_config = yaml_config["postprocess"];
  // Traverse postprocess root node branches
  for (const auto &postprocess_function_config : postprocess_config) {
	auto function_name = postprocess_function_config.begin()->first.as<std::string>();
	if (function_name == "landmark_num") {
	  auto &landmark_num_node = postprocess_function_config.begin()->second;
	  if (!landmark_num_node) {
		BREEZE_DEPLOY_LOGGER_ERROR("The function(landmark_num) must have a landmark_num element.")
		return false;
	  }
	  landmark_num_ = landmark_num_node.as<int>();
	} else {
	  BREEZE_DEPLOY_LOGGER_ERROR("The postprocess name only supports [landmark_num], "
								 "but now it is called {}.", function_name)
	  return false;
	}
  }
  return true;
}
bool DetectionModel::Postprocess() {
  return true;
}
}
}