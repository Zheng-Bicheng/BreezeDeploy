# 以图搜图

```bash
install_name_tool -add_rpath ${PWD} test_image_recognition
test_image_recognition BreezeDeployModelZoo/pipeline/image_recognition/models/yolov5s-face.onnx \
                        config/detection/yolov5_onnx_template.yaml \
                        BreezeDeployModelZoo/pipeline/image_recognition/models/ms1mv3_arcface_r18.onnx \
                        config/feature/arcface_onnx_template.yaml \
                        BreezeDeployModelZoo/pipeline/image_recognition/database/face \
                        BreezeDeployModelZoo/pipeline/image_recognition/images/15911604352.jpg
```