# InsightFace-SCRFD 部署例程

SCRFD模型在RK开发板上部署主要分为以下三步：

* 步骤1：使用InsightFace训练模型、导出为ONNX模型
* 步骤2：使用rknn_toolkit2量化并转换为RKNN模型
* 步骤3：编译并运行代码

关于步骤1，请参考[InsightFace-SCRFD Repo](https://github.com/deepinsight/insightface/tree/master/detection/scrfd)，我这里直接使用了已经导出的ONNX模型。

在开始运行例程前，你需要在代码根目录下准备一个文件夹来存放你需要运行的模型。例如，你可以运行以下代码来准备一个文件夹

```bash
cd /path/to/BreezeDeploy
mkdir SCRFD_Model
mkdir SCRFD_Model/image
mkdir SCRFD_Model/model
touch SCRFD_Model/picture_lists.txt
```

运行代码后，你将得到一个 **SCRFD_Model** ，文件夹目录结构如下:

```text
SCRFD_Model/
├── image
├── model
└── picture_lists.txt
```

# 1 SCRFD(人脸检测版本部署)

接着运行以下代码来准备转换RKNN模型所需要的模型、图片、数据集文件

```bash
# Download model
cd /path/to/BreezeDeploy
wget https://bj.bcebos.com/paddlehub/fastdeploy/scrfd_500m_shape640x640.onnx -P SCRFD_Model/model
wget https://raw.githubusercontent.com/DefTruth/lite.ai.toolkit/main/examples/lite/resources/test_lite_face_detector_3.jpg -P SCRFD_Model/image
```

**scrfd**

```bash
export LD_LIBRARY_PATH=${PWD}
./test_scrfd BreezeDeployModelZoo/detection/models/scrfd_500m_shape640x640.onnx \
             config/detection/scrfd_template.yaml \
             BreezeDeployModelZoo/detection/images/test_lite_face_detector_3.jpg
```

# 2 SCRFD(人脸检测+关键点定位)

**scrfd**

```bash
export LD_LIBRARY_PATH=${PWD}
./test_scrfd BreezeDeployModelZoo/detection/models/scrfd_500m_bnkps_shape640x640.onnx \
             config/detection/scrfd_template.yaml \
             BreezeDeployModelZoo/detection/images/test_lite_face_detector_3.jpg
```