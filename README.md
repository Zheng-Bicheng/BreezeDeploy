# BreezeDeploy

**BreezeDeploy**参考FastDeploy设计而来，是一款在RK平台上主打高效和易用的AI推理部署工具。
BreezeDeploy在FastDeploy的基础上，对内存拷贝、前后处理、模型推理等多个方面进行了优化。
希望BreezeDeploy能够帮助你更快的部署CV模型。

# 编译BreezeDeploy

```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=${PWD}/install
make -j8
make install
```