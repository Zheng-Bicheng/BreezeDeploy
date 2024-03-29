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
#include "breeze_deploy/core/time/breeze_deploy_time.h"
#include "breeze_deploy/models/classification/classification_model.h"

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main(int argc, char *argv[]) {
  if (argc != 4) {
	std::cout << "Usage: test_label_result path/to/model /path/to/config_file path/to/image" << std::endl;
	return -1;
  }

  std::string model_path = argv[1];
  std::string config_path = argv[2];

  ClassificationModel label_model(model_path, config_path);
  if (!label_model.Initialize()) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }

  ClassificationResult result;
  std::string image_path = argv[3];
  auto mat = cv::imread(image_path);
  BreezeDeployTime cost;
  cost.Start();
  if (!label_model.Predict(mat, result)) {
    std::cout << "模型推理失败" << std::endl;
    return 1;
  }
  cost.End();
  cost.PrintInfo("LabelModel", 1.0 / 100, BreezeDeployTimeType::Milliseconds);

  printf("TopK: %zu\n", result.GetSize());
  for (int i = 0; i < result.GetSize(); ++i) {
	printf("Label ID is %ld, confidence is %f\n", result.topk_label_id_vector[i], result.topk_confidence_vector[i]);
  }
  return 0;
}