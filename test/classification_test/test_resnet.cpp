#include <iostream>
#include <string>
#include "breeze_deploy/core/breeze_deploy_time.h"
#include "breeze_deploy/models/classification/resnet/resnet.h"

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main(int argc, char *argv[]) {
  if (argc < 4) {
	std::cout << "Usage: test_resnet path/to/model /path/to/config_file path/to/image path/to/label" << std::endl;
	return -1;
  }

  std::string model_path = argv[1];
  std::string config_path = argv[2];
  Resnet resnet(model_path, config_path);
  if (!resnet.Initialize()) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }
  std::string label_file_path = argv[4];
  resnet.SetLabel(label_file_path);

  std::string image_path = argv[3];
  auto mat = cv::imread(image_path);
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

  auto classification_results = resnet.GetClassificationResults();
  for (auto &classification_result : classification_results) {
	printf("Label is %s,confidence is %f\n", classification_result.label.c_str(), classification_result.confidence);
  }
  return 0;
}