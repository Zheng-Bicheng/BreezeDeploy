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
#include <iostream>
#include <string>
#include "breeze_deploy/core/breeze_deploy_time.h"
#include "breeze_deploy/models/classification/ghostnet/ghost_net.h"

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main(int argc, char *argv[]) {
  if (argc < 4) {
	std::cout << "Usage: test_ghostnet path/to/model /path/to/config_file path/to/image path/to/label" << std::endl;
	return -1;
  }

  std::string model_path = argv[1];
  std::string config_path = argv[2];

  GhostNet ghost_net(model_path, config_path);
  if (!ghost_net.Initialize()) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }
  std::string label_file_path = argv[4];
  ghost_net.SetLabel(label_file_path);

  std::string image_path = argv[3];
  auto mat = cv::imread(image_path);
  BreezeDeployTime cost;
  cost.Start();
  for (int i = 0; i < 100; ++i) {
	if (!ghost_net.Predict(mat)) {
	  std::cout << "模型推理失败" << std::endl;
	  return 1;
	}
  }
  cost.End();
  cost.PrintInfo("GhostNet", 1.0 / 100, BreezeDeployTimeType::Milliseconds);

  auto classification_results = ghost_net.GetClassificationResults();
  for (auto &classification_result : classification_results) {
	printf("Label is %s,confidence is %f\n", classification_result.label.c_str(), classification_result.confidence);
  }
  return 0;
}