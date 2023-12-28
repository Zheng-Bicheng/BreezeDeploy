#include <iostream>
#include <string>
#include <breeze_deploy/models/classification/resnet/resnet.h>

using namespace breeze_deploy;
using namespace breeze_deploy::models;
using cv::imread;

int main() {
  auto image_path = "/Users/zhengbicheng/CLionProjects/BreezeDeploy/test/images/ILSVRC2012_val_00020010.jpg";
  auto mat = cv::imread(image_path);

  auto model_path = "/Users/zhengbicheng/CLionProjects/BreezeDeploy/test/model/resnet50-v2-7.onnx";
  BreezeDeployBackendOption breeze_deploy_backend_option;
  breeze_deploy_backend_option.SetModelPath(model_path);

  Resnet resnet;
  if (!resnet.Initialize(breeze_deploy_backend_option)) {
	std::cout << "模型初始化失败" << std::endl;
	return 1;
  }

  auto label_file_path = "/Users/zhengbicheng/CLionProjects/BreezeDeploy/test/label/ILSVRC2012_label.txt";
  resnet.SetLabel(label_file_path);

  if (!resnet.Predict(mat)) {
	std::cout << "模型推理失败" << std::endl;
	return 1;
  }

  auto results_vector_ = resnet.GetClassificationResult();
  printf("Label is %s,confidence is %f\n", results_vector_[0].label_.c_str(), results_vector_[0].confidence_);
  return 0;
}