<h1 align="center">BreezeDeploy</h1>

<p align="center">
    <a href="./LICENSE"><img src="https://img.shields.io/badge/license-Apache%202-dfd.svg"></a>
    <a href="https://github.com/Zheng-Bicheng/BreezeDeploy/releases"><img src="https://img.shields.io/badge/release-release%2F0.2.0-yellow"></a>
    <a href=""><img src="https://img.shields.io/badge/os-win,linux-pink.svg"></a>
</p>

<p align="center">
    <a href=""><b> 驱动安装文档 </b></a>
    |
    <a href="./example/README.md"><b> 二次开发例程文档 </b></a>
    |
    <a href=""><b> 二次开发API文档 </b></a>
</p>

BreezeDeploy是一款用于在RK平台上高效部署AI视觉模型的部署工具库。除部分对NPU极度不友好的模型外，BreezeDeploy计划全程使用RKNPU进行推理。
为了提供高效、灵活且可靠的部署解决方案，BreezeDeploy采用C++编写并计划提供Python工具包。

# 📌 稳定版本

BreezeDeploy最新的稳定版本为**release/0.2.0**
，如果您有固定版本的需求，你可以前往[Release](https://github.com/Zheng-Bicheng/BreezeDeploy/releases)来使用指定的稳定版本。

> ⚠️值得注意的是，BreezeDeploy正在高速迭代中，我们强烈建议您编译最新的**develop**分支的代码以获取更好的使用体验，而不是使用现有的稳定版本。

# ℹ️ 简介

[FastDeploy](https://github.com/PaddlePaddle/FastDeploy/tree/develop)是一款全场景、易用灵活、极致高效的AI推理部署工具，被很多用户所使用。
[BreezeDeploy](https://github.com/Zheng-Bicheng/BreezeDeploy)参考FastDeploy设计而来。
在FastDeploy的基础上， BreezeDeploy充分利用了部署平台的优势，对**内存拷贝**、**前后处理**、**模型推理**
等多个方面进行了优化，可以支持在RK平台上以较高的效率部署市面上常见的视觉算法。
希望它能够帮你实现快速且友好的模型部署体验。

# 🚤 快速编译

```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=${PWD}/breeze_deploy
make -j8
make install
```

# 🐛 Bug反馈

非常感谢您使用BreezeDeploy仓库！目前我们仍处于研发阶段，可能会遇到一些Bug。
如果您在使用过程中发现任何问题，请前往Issues页面反馈，我们将尽力解决并改进。感谢您的理解和支持！