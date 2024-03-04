# 人脸识别系统

人脸识别系统在RK开发板上部署主要分为以下三步：

* 步骤1：获取人脸识别模型，请参考[BreezeDeploy InsightFace-SCRFD Demo](../detection_test/scrfd.md)
* 步骤2：获取人脸检测模型，请参考[BreezeDeploy InsightFace-ArcFace Demo](../feature_test/arcface.md)
* 步骤3：编译并运行代码

# 1 准备模型文件夹

在开始运行例程前，你需要在代码根目录下准备一个文件夹来存放你需要运行的模型。例如，你可以运行以下代码来准备一个文件夹:

```bash
cd /path/to/BreezeDeploy
mkdir Face_Model
mkdir Face_Model/image
mkdir Face_Model/model
```

将准备好的rknn模型拷贝到 **Face_Model/model** 目录下

```bash
cp ArcFace_Model/model/ms1mv3_arcface_r18.rknn Face_Model/model
cp SCRFD_Model/model/scrfd_500m_shape640x640.rknn Face_Model/model
```

将你准备好的，想要用于识别的图片放到Face_Model/image目录下。

```bash
wget https://bj.bcebos.com/paddlehub/fastdeploy/rknpu2/face_demo.zip
unzip face_demo.zip
cp face_0.jpg Face_Model/image
cp face_1.jpg Face_Model/image
cp face_2.jpg Face_Model/image
```

```text
Face_Model/
├── image
│   ├── face_0.jpg
│   ├── face_1.jpg
│   └── face_2.jpg
└── model
    ├── ms1mv3_arcface_r18.rknn
    └── scrfd_500m_shape640x640.rknn

```


# 2 编译

```bash
mkdir build
cd build
cmake .. -DVCPKG_TARGET_TRIPLET="arm64-cross-compilation" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${PWD}/breeze_deploy_output"
make -j8
make install
```

# 3 运行程序

拷贝 **Face_Model** 到 **/path/to/BreezeDeploy/build/breeze_deploy_output/bin** ，并将 **breeze_deploy_output** 文件夹拷贝至开发板。

```bash
export LD_LIBRARY_PATH=${PWD}
# For ONNX
./test_image_recognition  Face_Model/model/scrfd_500m_shape640x640.onnx \
                          config/detection/scrfd_template.yaml \
                          Face_Model/model/ms1mv3_arcface_r18.onnx \
                          config/feature/arcface_template.yaml \
                          Face_Model/image/face_0.jpg \
                          Face_Model/image/face_2.jpg \
                          Face_Model/image/face_1.jpg
# For RKNN
./test_image_recognition  Face_Model/model/scrfd_500m_shape640x640.rknn \
                          config/detection/scrfd_template.yaml \
                          Face_Model/model/ms1mv3_arcface_r18.rknn \
                          config/feature/arcface_template.yaml \
                          Face_Model/image/face_0.jpg \
                          Face_Model/image/face_2.jpg \
                          Face_Model/image/face_1.jpg
```