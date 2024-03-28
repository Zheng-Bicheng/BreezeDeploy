# 人像分割

```bash
export LD_LIBRARY_PATH=${PWD}

wget https://paddleseg.bj.bcebos.com/dygraph/pp_humanseg_v2/images.zip
unzip -qo images.zip

# For ONNX
./test_segmentation  inference.onnx \
                      config/segmentation/paddle_segmentation_template.yaml \
                      images/portrait_heng.jpg
                      
# For RKNN
./test_segmentation  Face_Model/model/scrfd_500m_shape640x640.rknn \
                          config/detection/scrfd_template.yaml \
                          Face_Model/model/ms1mv3_arcface_r18.rknn \
                          config/feature/arcface_template.yaml \
                          Face_Model/image/face_0.jpg \
                          Face_Model/image/face_2.jpg \
                          Face_Model/image/face_1.jpg
```