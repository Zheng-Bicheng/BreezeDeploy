# 特征提取

```bash
#For Linux
export LD_LIBRARY_PATH=${PWD}
#For Mac
install_name_tool -add_rpath ${PWD} test_label_result
#Run
./test_feature_result BreezeDeployModelZoo/classfication/models/ms1mv3_arcface_r18.onnx \
                      config/feature/arcface_onnx_template.yaml \
                      BreezeDeployModelZoo/classfication/images/face_0.jpg \
                      BreezeDeployModelZoo/classfication/images/face_1.jpg
```