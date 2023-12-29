project(test)

set(INSTALL_APP_LIST "")

# test_resnet
add_executable(test_resnet classification_test/test_resnet.cpp)
target_link_libraries(test_resnet breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_resnet)

add_executable(test_ghostnet classification_test/test_ghostnet.cpp)
target_link_libraries(test_ghostnet breeze_deploy ${OpenCV_LIBS})
list(APPEND INSTALL_APP_LIST test_ghostnet)

# install
install(TARGETS ${INSTALL_APP_LIST} DESTINATION bin)