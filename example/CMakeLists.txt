project(test)

set(INSTALL_APP_LIST "")

# test_label_result
add_executable(test_label_result classification_test/test_label_result.cpp)
target_link_libraries(test_label_result breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_label_result)

# test_yolov5
add_executable(test_yolov5 detection_test/test_yolov5.cpp)
target_link_libraries(test_yolov5 breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_yolov5)
# test_yolov5_face
add_executable(test_yolov5_face detection_test/test_yolov5_face.cpp)
target_link_libraries(test_yolov5_face breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_yolov5_face)

# test_yolov5_face_license_plate
#add_executable(test_yolov5_face_license_plate pipeline/test_yolov5_face_license_plate.cpp)
#target_link_libraries(test_yolov5_face_license_plate breeze_deploy ${OpenCV_LIBS})
#list(APPEND INSTALL_APP_LIST test_yolov5_face_license_plate)

# install
install(TARGETS ${INSTALL_APP_LIST} DESTINATION bin)