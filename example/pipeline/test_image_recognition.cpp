// Copyright (c) 2024/1/23 Zheng-Bicheng. All Rights Reserved.
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
#include "breeze_deploy/models/classification/arcface/arc_face.h"
#include "breeze_deploy/models/detection/yolov5_face/yolov_5_face.h"
#include "breeze_deploy/pipeline/image_recognition/image_recognition.h"

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main(int argc, char *argv[]) {
#if 0
  if (argc < 3) {
	std::cout << "Usage: test_image_recognition path/to/model /path/to/config_file path/to/image" << std::endl;
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
  if (!detect_model.Predict(mat)) {
	std::cout << "模型推理失败" << std::endl;
	return 1;
  }
  cost.End();
  cost.PrintInfo("YOLOV5", 1.0, BreezeDeployTimeType::Milliseconds);

  auto detection_results = detect_model.GetDetectionResults();
  for (auto detection_result : detection_results) {
	detection_result.PrintResult();
  }
  mat = DetectionModel::Draw(mat, detection_results);
  cv::imwrite("./detect_result.png", mat);
#endif
  if (argc < 4) {
	std::cout
		<< "Usage: test_image_recognition "
		   "path/to/model_det /path/to/config_det "
		   "path/to/model_cls /path/to/config_cls "
		   "path/to/data_base" << std::endl;
	return -1;
  }

  std::string model_det_path = argv[1];
  std::string config_det_path = argv[2];
  std::string model_cls_path = argv[3];
  std::string config_cls_path = argv[4];
  auto image_recognition = ImageRecognition(std::make_unique<YOLOV5Face>(model_det_path, config_det_path),
											std::make_unique<ArcFace>(model_cls_path, config_cls_path));

  std::string data_base_path = argv[5];
  image_recognition.BuildDatabase(data_base_path, false);

  return 0;
}