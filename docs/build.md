# BreezeDeploy 安装指南

本界面提供如何从零构建BreezeDeploy软件包的说明，它主要包含两个步骤：

* 正确下载BreezeDeploy
* 正确安装vcpkg并使用vcpkg安装BreezeDeploy的依赖库
* 编译BreezeDeploy的C++共享库

## 1 正确下载BreezeDeploy

首先，请运行以下代码，从 [BreezeDeploy Github Repo](https://github.com/Zheng-Bicheng/BreezeDeploy/tree/develop) 上拉取最新的BreezeDeploy代码。

```bash
sudo apt install git
git clone https://github.com/Zheng-Bicheng/BreezeDeploy.git

cd BreezeDeploy
git submodule init
git submodule update
```

## 2 正确安装vcpkg

### 2.1 安装vcpkg

```bash
sudo apt-get install build-essential tar curl zip unzip
cd BreezeDeploy
./third_party/vcpkg/bootstrap-vcpkg.sh
```

### 2.2 安装交叉编译工具

为了能够在x86平台编译BreezeDeploy，我们一般需要使用交叉编译工具进行编译。请从 [arm-gnu-toolchain-downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) 下载交叉编译工具链，一般来说，建议使用软件版本为 **Downloads:11.2-2022.02** 的编译工具。

> 注意:
> 1 Ubuntu自带的交叉编译工具可能存在glibc依赖版本过高的问题，建议从Arm官网下载交叉编译工具链。
> 2 建议交叉编译工具链的版本不要低于我们指定的版本，否则BreezeDeploy的部分依赖包可能无法编译通过。

正确下载交叉编译工具后，我们需要解压并正确配置交叉编译工具相关的环境变量。

```bash
# 我这里将压缩包存放在了 ~/opt 目录下
cd ~/opt

# 解压交叉编译工具压缩包
tar -xvf gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu.tar.xz
```

使用gedit打开 **~/.bashrc** 文件并在最后添加以下代码:

```bash
# gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu
export PATH="$HOME/opt/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin:$PATH"
```

刷新环境变量

```bash
source ~/.bashrc
```

## 3 使用vcpkg安装BreezeDeploy的依赖库

```bash
sudo apt install gfortran
sudo apt install autoconf libtool
sudo apt-get install nasm

cd BreezeDeploy

# For amd64-linux
# ./third_party/vcpkg/vcpkg install --triplet=x64-linux spdlog eigen3 opencv4[core,eigen,dnn,jpeg,png] yaml-cpp faiss

# For arm64-linux
./third_party/vcpkg/vcpkg install --triplet=arm64-cross-compilation spdlog eigen3 opencv4[core,eigen,dnn,jpeg,png] yaml-cpp faiss
```

## 4 编译BreezeDeploy的C++共享库

```bash
cd BreezeDeploy

# For x64-linux
# mkdir build-x64
# cd build-x64
# cmake .. -DVCPKG_TARGET_TRIPLET="x64-linux"  -DENABLE_RKNN_RUNTIME=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${PWD}/breeze_deploy_output

# For arm64-linux
mkdir build-arm64
cd build-arm64
cmake .. -DVCPKG_TARGET_TRIPLET="arm64-cross-compilation" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${PWD}/breeze_deploy_output"

make -j8
make install
```