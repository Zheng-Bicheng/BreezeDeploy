<h1 align="center">BreezeDeploy例程文档</h1>

**example**目录下存放了BreezeDeploy的例程。为了能够正确的编译测试例程，请确保您在编译BreezeDeploy时打开了BUILD_TEST选项。

# 1⬇️ 下载BreezeDeployModelZoo

由于我们没有一个稳定的CDN，无法给出模型的下载直链，我们将**测试模型**、**测试数据**、**测试标签**等内容整合成了
**BreezeDeployModelZoo**文件夹。
你可以前往[OneDriver[密码: BreezeDeploy]](https://1drv.ms/f/s!AhN_zZGLHpt-ghTfJE9mDX9W1ISm)下载我们提供的测试内容。

> 下载完成后，请将**BreezeDeployModelZoo**目录移动到**install**指定的目录下。

# 2🌰 边端测部署能力速览

BreezeDeploy目前已经支持了**通用目标分类**、**通用目标检测**、**人脸检测模型**、**人脸特征提取模型**的部署。

(1) ✅ : 已经支持; (2) 🏃: 正在进行中; (3) N/A : 暂不支持.

| 任务场景   | 操作系统  | RK356X | RK3588 | 例程链接                           |
|--------|-------|--------|--------|--------------------------------|
| 通用分类   | Linux | 🏃     | ✅      | [C++代码](./classification_test) |
| 人脸特征提取 | Linux | 🏃     | ✅      | [C++代码](./feature_test)        |
| 通用检测   | Linux | 🏃     | ✅      | [C++代码](./detection_test)      |
| 人脸检测   | Linux | 🏃     | ✅      | [C++代码](./detection_test)      |
| 人脸识别系统 | Linux | 🏃     | ✅      | [C++代码](./pipeline_test)       |

