#include <iostream>
#include <string>
#include "breeze_deploy/core/breeze_deploy_time.h"
#include "breeze_deploy/models/classification/ghostnet/ghost_net.h"

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main(int argc, char *argv[]) {
  if (argc < 3) {
	std::cout << "Usage: rknpu_test path/to/model path/to/image path/to/label" << std::endl;
	return -1;
  }

  std::string model_path = argv[1];
  std::string image_path = argv[2];
  std::string label_file_path = argv[3];

  BreezeDeployBackendOption breeze_deploy_backend_option;
  breeze_deploy_backend_option.SetModelPath(model_path);

  GhostNet ghost_net;
  if (!ghost_net.Initialize(breeze_deploy_backend_option)) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }
  ghost_net.SetLabel(label_file_path);

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

  auto results_vector_ = ghost_net.GetClassificationResult();
  printf("Label is %s,confidence is %f\n", results_vector_[0].label_.c_str(), results_vector_[0].confidence_);
  return 0;
}