project(test)

set(INSTALL_APP_LIST "")

# test_label_result
add_executable(test_label_result classification_test/test_label_result.cpp)
list(APPEND INSTALL_APP_LIST test_label_result)

# test_feature_result
add_executable(test_feature_result feature_test/test_feature_result.cpp)
list(APPEND INSTALL_APP_LIST test_feature_result)

# test_yolov5
add_executable(test_yolov5 detection_test/test_yolov5.cpp)
list(APPEND INSTALL_APP_LIST test_yolov5)

# test_yolov5_face
add_executable(test_yolov5_face detection_test/test_yolov5_face.cpp)
list(APPEND INSTALL_APP_LIST test_yolov5_face)

# test_scrfd
add_executable(test_scrfd detection_test/test_scrfd.cpp)
list(APPEND INSTALL_APP_LIST test_scrfd)

# test_image_recognition
add_executable(test_image_recognition pipeline_test/test_image_recognition.cpp)
list(APPEND INSTALL_APP_LIST test_image_recognition)

# link and include
foreach(test_project ${INSTALL_APP_LIST})
    target_include_directories(${test_project} PRIVATE ${RUNTIME_INCLUDE_DIRS})
    target_link_libraries(${test_project} PRIVATE ${CMAKE_PROJECT_NAME} ${RUNTIME_LIBRARIES} ${OPENCV_LIBRARIES})
endforeach()

# install
install(TARGETS ${INSTALL_APP_LIST} DESTINATION bin)