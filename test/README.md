# BreezeDeploy 测试文档

该文档仅供内部测试使用

# 1 下载测试ModelZoo

测试ModelZoo中包含了测试模型、测试图片和测试标签，请下载后放置于install目录下

# 2 执行测试

进入install目录执行测试

```bash
# if mac
install_name_tool -add_rpath ${PWD} test_resnet 
# 
./test_resnet BreezeDeployModelZoo/classfication/models/ResNet50_vd_QAT.onnx config/classification/classification_onnx_template.yaml BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt
./test_ghostnet BreezeDeployModelZoo/classfication/models/GhostNet_x1_0_QAT.onnx config/classification/classification_onnx_template.yaml BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt

```