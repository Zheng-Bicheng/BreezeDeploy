// Copyright (c) 2024/1/30 Zheng-Bicheng. All Rights Reserved.
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

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

bool InferByONNX(ArcFace &arc_face, const std::string &image_path, ClassificationFeatureResult &result) {
  auto image = cv::imread(image_path);
  BreezeDeployTime cost;
  cost.Start();
  if (!arc_face.Predict(image, result)) {
	std::cout << "模型推理失败" << std::endl;
	return false;
  }
  cost.End();
  cost.PrintInfo("FeatureModel", 1.0, BreezeDeployTimeType::Milliseconds);
  return true;
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
	std::cout << "Usage: test_feature_result path/to/model /path/to/config_file path/to/image_0 path/to/image_1"
			  << std::endl;
	return -1;
  }

  std::string model_path = argv[1];
  std::string config_path = argv[2];

  ArcFace feature_model(model_path, config_path);
  if (!feature_model.Initialize()) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }

  ClassificationFeatureResult result0, result1;
  InferByONNX(feature_model, argv[3], result0);
  InferByONNX(feature_model, argv[4], result1);

  printf("相似度为: %lf\n", ClassificationModel::CosineSimilarity(result0, result1));
  return 0;
}