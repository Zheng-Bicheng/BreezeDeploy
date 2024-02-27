```bash
python tools/onnx2rknn.py --onnx_model_path BreezeDeployModelZoo/classfication/models/GhostNet_x1_0_QAT.onnx \
                          --config_file_path config/classification/GhostNet_x1_0_QAT_template.yaml \
                          --rknn_model_path ${PWD}/GhostNet_x1_0_QAT.rknn
                          
python tools/onnx2rknn.py --onnx_model_path BreezeDeployModelZoo/classfication/models/resnet18.onnx \
                          --config_file_path config/classification/ResNet18_template.yaml \
                          --rknn_model_path ${PWD}/resnet18.rknn
                          
python tools/onnx2rknn.py --onnx_model_path BreezeDeployModelZoo/classfication/models/ResNet50_vd_QAT.onnx \
                          --config_file_path config/classification/ResNet50_vd_QAT_template.yaml \
                          --rknn_model_path ${PWD}/ResNet50_vd_QAT.rknn
```