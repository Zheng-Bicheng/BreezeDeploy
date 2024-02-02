```bash
python tools/onnx2rknn.py --onnx_model_path BreezeDeployModelZoo/classfication/models/mobilenetv2-12.onnx\
                          --config_file_path config/classification/resnet_rknn_template.yaml \
                          --rknn_model_path ${PWD}/mobilenetv2-12.rknn
```