# 目标检测

```bash
export LD_LIBRARY_PATH=${PWD}
./test_yolov5 BreezeDeployModelZoo/detection/models/yolov5n.onnx \
              config/detection/yolov5/yolov5_onnx_template.yaml \
              BreezeDeployModelZoo/detection/images/000000014439.jpg
```

**scrfd**

```bash
export LD_LIBRARY_PATH=${PWD}
./test_scrfd BreezeDeployModelZoo/detection/models/scrfd_500m_shape640x640.onnx \
             config/detection/scrfd_template.yaml \
             BreezeDeployModelZoo/detection/images/test_lite_face_detector_3.jpg
```

# 目标检测+关键点定位

**yolov5s-face**

```bash
export LD_LIBRARY_PATH=${PWD}
./test_yolov5_face BreezeDeployModelZoo/detection/models/yolov5s-face.onnx \
                    config/detection/yolov5_face_onnx_template.yaml \
                    BreezeDeployModelZoo/detection/images/test_lite_face_detector_3.jpg
```

**scrfd**

```bash
export LD_LIBRARY_PATH=${PWD}
./test_scrfd BreezeDeployModelZoo/detection/models/scrfd_500m_bnkps_shape640x640.onnx \
             config/detection/scrfd_template.yaml \
             BreezeDeployModelZoo/detection/images/test_lite_face_detector_3.jpg
```