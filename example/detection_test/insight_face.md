# InsightFace-SCRFD 部署例程

SCRFD模型在RK开发板上部署主要分为以下三步：

* 步骤1：使用InsightFace训练模型、导出为ONNX模型
* 步骤2：使用rknn_toolkit2量化并转换为RKNN模型
* 步骤3：编译并运行代码

关于步骤1，请参考[InsightFace-SCRFD Repo](https://github.com/deepinsight/insightface/tree/master/detection/scrfd)，我这里直接使用了已经导出的ONNX模型。

# 1 准备模型文件夹

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

将你准备好的，想要用于量化训练的图片放到SCRFD_Model/image目录下。
我这里使用了[AI Studio-人脸识别](https://aistudio.baidu.com/datasetdetail/94187)，解压后，我的image目录结构如下:

```text
image/
├── dilireba
├── jiangwen
├── lipeiyu
├── pengyuyan
├── zhangziyi
└── zhaoliying
```

将你准备好的图片以SCRFD_Model为根目录，将路径写入picture_lists.txt中，例如:

```text
image/dilireba/15911604352.jpg
```

# 2 转换RKNN模型

为了节约时间，笔者没有自己转换SCRFD模型的ONNX模型，选择使用FastDeploy提供的已经转换好的ONNX模型。
你可以前往[FastDeploy example SCRFD](https://github.com/PaddlePaddle/FastDeploy/tree/develop/examples/vision/facedet/scrfd)下载自己喜欢的模型。

## 2.1 SCRFD(仅人脸检测)

接着运行以下代码来准备转换RKNN模型所需要的模型、图片、数据集文件

```bash
cd /path/to/BreezeDeploy
# Download model
wget https://bj.bcebos.com/paddlehub/fastdeploy/scrfd_500m_shape640x640.onnx -P SCRFD_Model/model
# Convert model
python tools/onnx2rknn.py --onnx_model_path SCRFD_Model/model/scrfd_500m_shape640x640.onnx \
                          --config_file_path config/detection/scrfd_template.yaml \
                          --rknn_model_path SCRFD_Model/model/scrfd_500m_shape640x640.rknn
```

## 2.2 SCRFD(人脸检测+关键点定位)

```bash
cd /path/to/BreezeDeploy
# Download model
wget https://bj.bcebos.com/paddlehub/fastdeploy/scrfd_500m_bnkps_shape640x640.onnx -P SCRFD_Model/model
# Convert model
python tools/onnx2rknn.py --onnx_model_path SCRFD_Model/model/scrfd_500m_bnkps_shape640x640.onnx \
                          --config_file_path config/detection/scrfd_template.yaml \
                          --rknn_model_path SCRFD_Model/model/scrfd_500m_bnkps_shape640x640.rknn
```

# 3 编译

```bash
mkdir build
cd build
cmake .. -DVCPKG_TARGET_TRIPLET="arm64-cross-compilation" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${PWD}/breeze_deploy_output"
make -j8
make install
```

# 4 运行程序

拷贝 **SCRFD_Model** 到 **/path/to/BreezeDeploy/build/breeze_deploy_output/bin** ，并将 **breeze_deploy_output** 文件夹拷贝至开发板。

## 4.1 SCRFD(仅人脸检测)

```bash
export LD_LIBRARY_PATH=${PWD}
./test_scrfd SCRFD_Model/model/scrfd_500m_shape640x640.rknn \
             config/detection/scrfd_template.yaml \
             SCRFD_Model/image/dilireba/15911604352.jpg
```

## 4.2 SCRFD(人脸检测+关键点定位)

```bash
export LD_LIBRARY_PATH=${PWD}
./test_scrfd SCRFD_Model/model/scrfd_500m_bnkps_shape640x640.rknn \
             config/detection/scrfd_template.yaml \
             SCRFD_Model/image/dilireba/15911604352.jpg
```