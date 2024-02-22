# 目标分类

```bash
# For Linux
export LD_LIBRARY_PATH=${PWD}
# For Mac
install_name_tool -add_rpath ${PWD} test_label_result
./test_label_result BreezeDeployModelZoo/classfication/models/GhostNet_x1_0_QAT.onnx \
                    config/classification/resnet_onnx_template.yaml \
                    BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg \
                    BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt
                    
./test_label_result BreezeDeployModelZoo/classfication/models/GhostNet_x1_0_QAT.rknn \
                    config/classification/GhostNet_x1_0_QAT_rknn_template.yaml \
                    BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg \
                    BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt
                    
./test_label_result BreezeDeployModelZoo/classfication/models/ResNet50_vd_QAT.rknn \
                    config/classification/resnet_rknn_template.yaml \
                    BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg \
                    BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt
                    
                    
./test_label_result BreezeDeployModelZoo/classfication/models/resnet18.onnx \
                    config/classification/resnet_onnx_template.yaml \
                    BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg \
                    BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt
                    
./test_label_result BreezeDeployModelZoo/classfication/models/resnet18.rknn \
                    config/classification/resnet18_rknn_template.yaml \
                    BreezeDeployModelZoo/classfication/images/ILSVRC2012_val_00020010.jpg \
                    BreezeDeployModelZoo/classfication/labels/ILSVRC2012_label.txt
```