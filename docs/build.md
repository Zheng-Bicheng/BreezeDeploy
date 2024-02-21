
## 从Github下载BreezeDeploy

```bash
sudo apt install git
git clone https://github.com/Zheng-Bicheng/BreezeDeploy.git

cd /path/to/BreezeDeploy
git submodule init
git submodule update
```

## 安装vcpkg

```bash
sudo apt-get install build-essential tar curl zip unzip
cd /path/to/BreezeDeploy/third_party
./vcpkg/bootstrap-vcpkg.sh

# 如果需要交叉编译，还需要安装交叉编译工具
sudo apt install gcc-aarch64-linux-gnu
sudo apt install g++-aarch64-linux-gnu
```

## 使用vcpkg安装指定的工具包

```bash
sudo apt install gfortran
sudo apt install autoconf libtool
sudo apt-get install nasm

# For amd64-linux
./vcpkg/vcpkg install --triplet=x64-linux spdlog eigen3 opencv4[core,eigen,dnn,jpeg,png] yaml-cpp faiss

# For arm64-linux
./vcpkg/vcpkg install --triplet=arm64-cross-compilation spdlog eigen3 opencv4[core,eigen,dnn,jpeg,png] yaml-cpp faiss
```

## 编译

```bash
# For amd64-linux
cmake .. -DVCPKG_TARGET_TRIPLET="x64-linux"  -DENABLE_RKNN_RUNTIME=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${PWD}/breeze_deploy

# For arm64-linux
cmake .. -DVCPKG_TARGET_TRIPLET="arm64-cross-compilation" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${PWD}/breeze_deploy"
```