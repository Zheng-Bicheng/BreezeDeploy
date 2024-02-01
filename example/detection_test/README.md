# 目标检测

```bash
# For Mac
install_name_tool -add_rpath ${PWD} test_yolov5
./test_yolov5 BreezeDeployModelZoo/detection/models/yolov5n.onnx \
              config/detection/yolov5/yolov5_onnx_template.yaml \
              BreezeDeployModelZoo/detection/images/000000014439.jpg
```

# 目标检测+关键点定位

```bash
# For Mac
install_name_tool -add_rpath ${PWD} test_yolov5_face
./test_yolov5_face BreezeDeployModelZoo/detection/models/yolov5s-face.onnx \
                    config/detection/yolov5_face/yolov5_face_onnx_template.yaml \
                    BreezeDeployModelZoo/detection/images/test_lite_face_detector_3.jpg
```