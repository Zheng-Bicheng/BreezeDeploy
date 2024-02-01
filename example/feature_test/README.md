# 特征提取

```bash
# For Mac
install_name_tool -add_rpath ${PWD} test_label_result
./test_feature_result BreezeDeployModelZoo/classfication/models/ms1mv3_arcface_r18.onnx \
                      config/classification/arcface_onnx_template.yaml \
                      BreezeDeployModelZoo/classfication/images/face_0.jpg \
                      BreezeDeployModelZoo/classfication/images/face_1.jpg
```