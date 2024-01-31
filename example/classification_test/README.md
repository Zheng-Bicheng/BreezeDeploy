# 目标分类

```bash
# For Mac
install_name_tool -add_rpath ${PWD} test_label_result
./test_label_result BreezeDeployModelZoo/classfication/models/GhostNet_x1_0_QAT.onnx \
                    config/classification/resnet_onnx_template.yaml \
                    BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg \
                    BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt
```

# 特征提取

```bash
# For Mac
install_name_tool -add_rpath ${PWD} test_label_result
./test_feature_result BreezeDeployModelZoo/classfication/models/ms1mv3_arcface_r18.onnx \
                      config/classification/arcface_onnx_template.yaml \
                      BreezeDeployModelZoo/classfication/images/face_0.jpg \
                      BreezeDeployModelZoo/classfication/images/face_1.jpg
```