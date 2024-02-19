CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/third_party/vcpkg" CACHE PATH "")
if (${VCPKG_TARGET_TRIPLET} STREQUAL "arm64-cross-compilation")
    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/toolchains/arm64-cross-compilation-toolchain.cmake")
endif ()
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

PROJECT(breeze_deploy LANGUAGES C CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)

include(cmake/opencv.cmake)
include(cmake/eigen.cmake)
include(cmake/spdlog.cmake)
include(cmake/yaml_cpp.cmake)
include(cmake/faiss.cmake)

option(ENABLE_ONNX_RUNTIME "Enable support for onnx runtime." ON)
if (ENABLE_ONNX_RUNTIME)
    add_definitions(-DENABLE_ONNX_RUNTIME)
    include(cmake/onnx_runtime.cmake)
    INCLUDE_DIRECTORIES(${ONNX_RUNTIME_INCLUDE_DIRS})
endif ()

option(ENABLE_RKNN_RUNTIME "Enable support for rknn runtime." ON)
if (ENABLE_RKNN_RUNTIME)
    add_definitions(-DENABLE_RKNN_RUNTIME)
    include(cmake/rknn_runtime.cmake)
    INCLUDE_DIRECTORIES(${RKNN_RUNTIME_INCLUDE_DIRS})
endif ()

# 设置编译对象
set(BREEZE_DEPLOY_SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/breeze_deploy)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
file(GLOB_RECURSE BREEZE_DEPLOY_HEADS ${BREEZE_DEPLOY_SOURCE_DIRECTORY}/*.h)
file(GLOB_RECURSE BREEZE_DEPLOY_SOURCES ${BREEZE_DEPLOY_SOURCE_DIRECTORY}/*.cpp)
add_library(${CMAKE_PROJECT_NAME} SHARED ${BREEZE_DEPLOY_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
        ${SPDLOG_LIBRARIES} ${FAISS_LIBRARIES} ${EIGEN_LIBRARIES}
        ${OPENCV_LIBRARIES} ${ONNX_RUNTIME_LIBRARIES} ${YAML_CPP_LIBRARIES} ${RKNN_RUNTIME_LIBRARIES}
)

# TODO: 判断编译时是否开启example
add_subdirectory(example)

# 安装动态库到lib目录
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION lib)
# 安装配置文件到config目录
install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION .)
# 安装测试程序到bin目录
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
install(FILES ${ONNX_RUNTIME_LIBRARIES} DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION bin)