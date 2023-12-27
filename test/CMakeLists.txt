project(test)

add_executable(test_resnet test_resnet.cpp)
target_link_libraries(test_resnet breeze_deploy ${OpenCV_LIBS})