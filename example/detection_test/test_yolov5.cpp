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
#include "breeze_deploy/models/detection/yolov5/yolov_5.h"

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main(int argc, char *argv[]) {
  if (argc < 3) {
	std::cout << "Usage: test_yolov5 path/to/model /path/to/config_file path/to/image" << std::endl;
	return -1;
  }

  std::string model_path = argv[1];
  std::string config_path = argv[2];
  YOLOV5 detect_model(model_path, config_path);
  if (!detect_model.Initialize()) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }

  detect_model.SetConfidenceThreshold(0.5);
  detect_model.SetNMSThreshold(0.5);

  std::string image_path = argv[3];
  auto mat = cv::imread(image_path);

  BreezeDeployTime cost;
  cost.Start();
  DetectionResult result;
  if (!detect_model.Predict(mat, result)) {
	std::cout << "模型推理失败" << std::endl;
	return 1;
  }
  cost.End();
  cost.PrintInfo("YOLOV5", 1.0, BreezeDeployTimeType::Milliseconds);
  mat = DetectionModel::Draw(mat, result);
  cv::imwrite("./detect_result.png", mat);
  return 0;
}