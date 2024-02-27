# InsightFace-SCRFD 部署例程

SCRFD模型在RK开发板上部署主要分为以下三步：

* 步骤1：使用InsightFace训练模型、导出为ONNX模型
* 步骤2：使用rknn_toolkit2量化并转换为RKNN模型
* 步骤3：编译并运行代码

关于步骤1，请参考[InsightFace-SCRFD Repo](https://github.com/deepinsight/insightface/tree/master/detection/scrfd)，我这里直接使用了已经导出的ONNX模型。

# 1 SCRFD 人脸检测版本部署

**scrfd**

```bash
export LD_LIBRARY_PATH=${PWD}
./test_scrfd BreezeDeployModelZoo/detection/models/scrfd_500m_shape640x640.onnx \
             config/detection/scrfd_template.yaml \
             BreezeDeployModelZoo/detection/images/test_lite_face_detector_3.jpg
```

# 2 人脸检测+关键点定位

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