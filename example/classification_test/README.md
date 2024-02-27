# 目标分类Demo

# 1 PaddleClas模型在RKNPU2上部署

PaddleClas模型转换到RKNN模型主要分为以下两部步：

* 步骤1：使用PaddleClas训练模型、使用PaddleSlim量化模型、导出为ONNX模型
* 步骤2：转换为RKNN模型

## 1.1 获取PaddleClas Paddle模型

关于如何得到PaddleClas模型，你可以参考【BreezeDeploy】在RKNPU2上部署PaddleClas模型](https://aistudio.baidu.com/projectdetail/7486651?contributionType=1&sUid=790375&shared=1&ts=1708938425368)

## 1.2 获取RKNN模型

当你跑通步骤1后，你将得到一个**BreezeDeploy_ResNet18**文件夹，请将其拷贝在BreezeDeploy根目录下。
接下来我们需要编写一个转换和部署脚本文件，你可以参考[resnet18_template.yaml](../../config/classification/resnet18_template.yaml)
来实现你的转换配置文件。

```yaml
# Before starting the inference, we need to preprocess the input data.
# Assuming our input data is of shape [h, w, c], the following parameters are its preprocessing parameters.
preprocess:
  # Resize the image to [1, 224, 224].
  - Resize:
      width: 224
      height: 224
  # Perform color channel transformation, converting RGB colors to BGR.
  - BGRToRGB:
  # Perform normalization operation.
  - Normalize:
      mean: &normalize_mean [ 0.485, 0.456, 0.406 ]
      std: &normalize_std [ 0.229, 0.224, 0.225 ]
  # The input to the ONNX model is in NCHW format, and we need to change the memory layout of the data from NHWC to NCHW.
  - HWCToCHW:

# After preprocessing, we obtain an input vector of [1, 3, 224, 224], and now we need to perform inference.
# The following parameters are the configuration settings for the RKNN backend:
rknn_export:
  - Config:
      mean_values: *normalize_mean
      std_values: *normalize_std
      target_platform: "RK3568"
  #      target_platform: "RK3588"
  - LoadONNX:
      inputs: ["inputs"]
      input_size_list: [[1,3,224,224]]
      input_initial_val: null
      outputs: null
  - Build:
      do_quantization: false
      dataset: null
      rknn_batch_size: null

# After the inference, we obtain an output vector of [1, 1000], and we need to perform post-processing on the vector.
# The following parameters are for post-processing.
postprocess:
  # If the Softmax operator is missing from your model's output, the top K elements may not represent confidence.
  # You also need to apply the Softmax operation to the top K elements.
  - Softmax: true
```

一般来说，你需要检查**preprocess/Normalize**配置参数是否正确以及**postprocess/Softmax**来控制是否在后处理时添加Softmax操作。

编写完脚本后，执行以下代码，转换ONNX模型到RKNN模型。
```bash
cd /path/to/BreezeDeploy
python tools/onnx2rknn.py --onnx_model_path BreezeDeploy_ResNet18/models/inference_int8.onnx \
                          --config_file_path config/classification/resnet18_template.yaml \
                          --rknn_model_path BreezeDeploy_ResNet18/models/resnet18.rknn
```

## 1.3 编译程序

```bash
mkdir build
cd build

# 交叉编译
cmake .. -DVCPKG_TARGET_TRIPLET="arm64-cross-compilation" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${PWD}/breeze_deploy_output"

make -j8
make install
```

## 1.4 运行例程

拷贝**BreezeDeploy_ResNet18**到**/path/to/BreezeDeploy/build/breeze_deploy_output/bin**，并将**breeze_deploy_output**文件夹拷贝至开发板。

开发板上执行以下命令:

```bash
cd /path/to/breeze_deploy_output/bin
export LD_LIBRARY_PATH=${PWD}
./test_label_result BreezeDeploy_ResNet18/models/resnet18.rknn \
                    config/classification/resnet18_template.yaml \
                    BreezeDeploy_ResNet18/images/COVID001.png
```