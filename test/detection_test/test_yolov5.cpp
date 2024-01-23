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
#include "breeze_deploy/models/detection/yolov5/yolov_5.h"

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main(int argc, char *argv[]) {
  if (argc < 4) {
	std::cout << "Usage: test_yolov5 path/to/model /path/to/config_file path/to/image path/to/label" << std::endl;
	return -1;
  }

  std::string model_path = argv[1];
  std::string config_path = argv[2];
  YOLOV5 detect_model(model_path, config_path);
  if (!detect_model.Initialize()) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }
//  std::string label_file_path = argv[4];
//  yolov_5.SetLabel(label_file_path);

  std::string image_path = argv[3];
  auto mat = cv::imread(image_path);
#if 0
  BreezeDeployTime cost;
  cost.Start();
  for (int i = 0; i < 100; ++i) {
	if (!resnet.Predict(mat)) {
	  std::cout << "模型推理失败" << std::endl;
	  return 1;
	}
  }
  cost.End();
  cost.PrintInfo("Resnet", 1.0 / 100, BreezeDeployTimeType::Milliseconds);
#else
  if (!detect_model.Predict(mat)) {
	std::cout << "模型推理失败" << std::endl;
	return 1;
  }
#endif
  auto detection_results = detect_model.GetDetectionResults();
  for (auto detection_result : detection_results) {
	detection_result.PrintResult();
  }
  mat = DetectionModel::Draw(mat, detection_results);
  cv::imwrite("./detect_result.png", mat);
//  auto classification_results = yolov_5.GetClassificationResults();
//  for (auto &classification_result : classification_results) {
//	printf("Label is %s,label_confidence_ is %f\n", classification_result.label_name_.c_str(), classification_result.label_confidence_);
//  }
  return 0;
}