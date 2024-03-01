# InsightFace-ArcFace 部署例程

ArcFace模型在RK开发板上部署主要分为以下三步：

* 步骤1：使用InsightFace训练模型、导出为ONNX模型
* 步骤2：使用rknn_toolkit2量化并转换为RKNN模型
* 步骤3：编译并运行代码

关于步骤1，请参考[InsightFace-ArcFace Repo](https://github.com/deepinsight/insightface/tree/master/recognition/arcface_torch)，我这里直接使用了已经导出的ONNX模型。

# 1 准备模型文件夹

在开始运行例程前，你需要在代码根目录下准备一个文件夹来存放你需要运行的模型。例如，你可以运行以下代码来准备一个文件夹

```bash
cd /path/to/BreezeDeploy
mkdir ArcFace_Model
mkdir ArcFace_Model/image
mkdir ArcFace_Model/model
touch ArcFace_Model/picture_lists.txt
```

运行代码后，你将得到一个 **ArcFace_Model** ，文件夹目录结构如下:

```text
ArcFace_Model/
├── image
├── model
└── picture_lists.txt
```

将你准备好的，想要用于量化训练的图片放到ArcFace_Model/image目录下。

```bash
wget https://bj.bcebos.com/paddlehub/fastdeploy/rknpu2/face_demo.zip
unzip face_demo.zip
cp face_0.jpg ArcFace_Model/image
cp face_1.jpg ArcFace_Model/image
cp face_2.jpg ArcFace_Model/image
```

> 这里face_0和face_1是同一人，face_2是另一人

将你准备好的图片以ArcFace_Model为根目录，将路径写入picture_lists.txt中，例如:

```text
image/face_0.jpg
image/face_1.jpg
image/face_2.jpg
```

# 2 转换RKNN模型

为了节约时间，笔者没有自己转换ArcFace模型的ONNX模型，选择使用FastDeploy提供的已经转换好的ONNX模型。
你可以前往[FastDeploy example InsightFace](https://github.com/PaddlePaddle/FastDeploy/tree/develop/examples/vision/faceid/insightface)下载自己喜欢的模型。


接着运行以下代码来准备转换RKNN模型所需要的模型、图片、数据集文件

```bash
cd /path/to/BreezeDeploy
# Download model
wget https://bj.bcebos.com/paddlehub/fastdeploy/ms1mv3_arcface_r18.onnx -P ArcFace_Model/model

# Convert model
python tools/onnx2rknn.py --onnx_model_path ArcFace_Model/model/ms1mv3_arcface_r18.onnx \
                          --config_file_path config/feature/arcface_template.yaml \
                          --rknn_model_path ArcFace_Model/model/ms1mv3_arcface_r18.rknn
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

拷贝 **ArcFace_Model** 到 **/path/to/BreezeDeploy/build/breeze_deploy_output/bin** ，并将 **breeze_deploy_output** 文件夹拷贝至开发板。

```bash
export LD_LIBRARY_PATH=${PWD}

# 判断是否为同一人
./test_feature_result ArcFace_Model/model/ms1mv3_arcface_r18.rknn \
                      config/feature/arcface_template.yaml \
                      ArcFace_Model/image/face_0.jpg \
                      ArcFace_Model/image/face_1.jpg

# 判断是否不为同一人
./test_feature_result ArcFace_Model/model/ms1mv3_arcface_r18.rknn \
                      config/feature/arcface_template.yaml \
                      ArcFace_Model/image/face_0.jpg \
                      ArcFace_Model/image/face_2.jpg
./test_feature_result ArcFace_Model/model/ms1mv3_arcface_r18.rknn \
                      config/feature/arcface_template.yaml \
                      ArcFace_Model/image/face_1.jpg \
                      ArcFace_Model/image/face_2.jpg
```