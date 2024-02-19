project(test)

set(INSTALL_APP_LIST "")

# test_label_result
add_executable(test_label_result classification_test/test_label_result.cpp)
target_link_libraries(test_label_result breeze_deploy)
list(APPEND INSTALL_APP_LIST test_label_result)

## test_feature_result
#add_executable(test_feature_result feature_test/test_feature_result.cpp)
#target_link_libraries(test_feature_result breeze_deploy)
#list(APPEND INSTALL_APP_LIST test_feature_result)
#
## test_yolov5
#add_executable(test_yolov5 detection_test/test_yolov5.cpp)
#target_link_libraries(test_yolov5 breeze_deploy)
#list(APPEND INSTALL_APP_LIST test_yolov5)
#
## test_yolov5_face
#add_executable(test_yolov5_face detection_test/test_yolov5_face.cpp)
#target_link_libraries(test_yolov5_face breeze_deploy)
#list(APPEND INSTALL_APP_LIST test_yolov5_face)
#
## test_image_recognition
#add_executable(test_image_recognition pipeline_test/test_image_recognition.cpp)
#target_link_libraries(test_image_recognition breeze_deploy)
#list(APPEND INSTALL_APP_LIST test_image_recognition)

# install
install(TARGETS ${INSTALL_APP_LIST} DESTINATION bin)