<h1 align="center">BreezeDeploy</h1>

<p align="center">
    <a href="./LICENSE"><img src="https://img.shields.io/badge/license-Apache%202-dfd.svg"></a>
    <a href="https://github.com/Zheng-Bicheng/BreezeDeploy/releases"><img src="https://img.shields.io/badge/release-release%2F0.0.1-yellow"></a>
    <a href=""><img src="https://img.shields.io/badge/os-win,linux-pink.svg"></a>
</p>

<p align="center">
    <a href=""><b> 驱动安装文档 </b></a>
    |
    <a href=""><b> 二次开发例程文档 </b></a>
    |
    <a href=""><b> 二次开发API文档 </b></a>
</p>

[FastDeploy](https://github.com/PaddlePaddle/FastDeploy/tree/develop)是一款全场景、易用灵活、极致高效的AI推理部署工具，被很多用户所使用。
**FastDeploy**为了兼容多后端、多平台和多架构，在性能方面做了很多的取舍，没有充分利用上部署平台的优势。
[BreezeDeploy](https://github.com/Zheng-Bicheng/BreezeDeploy)参考FastDeploy设计而来，是一款在RK平台上主打高效和易用的AI推理部署工具。
BreezeDeploy在FastDeploy的基础上，对内存拷贝、前后处理、模型推理等多个方面进行了优化，同时充分利用了部署平台的优势，希望它能够帮助你实现快速部署。

# 🚤快速编译

```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=${PWD}/breeze_deploy
make -j8
make install
```

# 📳边端测部署能力速览

(1) ✅ : 已经支持; (2) ❔: 正在进行中; (3) N/A : 暂不支持.

| 任务场景 | 操作系统  | RK356X | RK3588 | 例程链接                                   |
|------|-------|--------|--------|----------------------------------------|
| 通用分类 | Linux | 🏃     | ✅      | [C++代码](./example/classification_test) |
| 人脸识别 | Linux | 🏃     | ✅      | [C++代码](./example/classification_test) |
| 通用检测 | Linux | 🏃     | ✅      | [C++代码](./example/detection_test)      |
| 人脸检测 | Linux | 🏃     | ✅      | [C++代码](./example/detection_test)      |


