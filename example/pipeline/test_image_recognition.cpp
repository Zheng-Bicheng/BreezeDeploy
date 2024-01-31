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
  auto image_recognition = ImageRecognition(std::make_unique<ArcFace>(model_cls_path, config_cls_path),
											std::make_unique<YOLOV5Face>(model_det_path, config_det_path)
  );

  if (!image_recognition.Initialize()) {
	std::cerr << "Initialization of Image Recognition failed. "
				 "Please check the configuration parameters for the detection model or recognition model." << std::endl;
	return -1;
  }

  std::string data_base_path = argv[5];
  image_recognition.BuildDatabase(data_base_path, false);

  return 0;
}