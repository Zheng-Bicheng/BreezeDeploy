project(test)

set(INSTALL_APP_LIST "")

# test_resnet
add_executable(test_resnet classification_test/test_resnet.cpp)
target_link_libraries(test_resnet breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_resnet)

# test_ghostnet
add_executable(test_ghostnet classification_test/test_ghostnet.cpp)
target_link_libraries(test_ghostnet breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_ghostnet)

# test_yolov5
add_executable(test_yolov5 detection_test/test_yolov5.cpp)
target_link_libraries(test_yolov5 breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_yolov5)
# test_yolov5_face
add_executable(test_yolov5_face detection_test/test_yolov5_face.cpp)
target_link_libraries(test_yolov5_face breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_yolov5_face)

# test_yolov5_face_license_plate
add_executable(test_yolov5_face_license_plate pipeline/test_yolov5_face_license_plate.cpp)
target_link_libraries(test_yolov5_face_license_plate breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_yolov5_face_license_plate)

# install
install(TARGETS ${INSTALL_APP_LIST} DESTINATION bin)