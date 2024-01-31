# 以图搜图

```bash
install_name_tool -add_rpath ${PWD} test_image_recognition
test_image_recognition BreezeDeployModelZoo/pipeline/image_recognition/models/yolov5s-face.onnx \
                        config/pipeline/image_recognition/yolov5_face_onnx_template.yaml \
                        BreezeDeployModelZoo/pipeline/image_recognition/models/ms1mv3_arcface_r18.onnx \
                        config/pipeline/image_recognition/arcface_onnx_template.yaml \
                        BreezeDeployModelZoo/pipeline/image_recognition/database/face
```