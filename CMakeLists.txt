CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/third_party/vcpkg" CACHE PATH "")
if (${VCPKG_TARGET_TRIPLET} STREQUAL "arm64-cross-compilation")
    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/toolchains/arm64-cross-compilation-toolchain.cmake")
endif ()
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_INSTALLED_ROOT "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
set(VCPKG_INCLUDE_ROOT "${VCPKG_INSTALLED_ROOT}/include")
set(VCPKG_LIBRARY_ROOT "${VCPKG_INSTALLED_ROOT}/lib")
set(VCPKG_SHARE_ROOT "${VCPKG_INSTALLED_ROOT}/share")
set(VCPKG_TOOLS_ROOT "${VCPKG_INSTALLED_ROOT}/tools")

PROJECT(breeze_deploy
        VERSION 0.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

include(cmake/opencv.cmake)
include(cmake/eigen.cmake)
include(cmake/spdlog.cmake)
include(cmake/yaml_cpp.cmake)
include(cmake/faiss.cmake)

set(RUNTIME_INCLUDE_DIRS "")
set(RUNTIME_LIBRARIES "")

option(ENABLE_ONNX_RUNTIME "Enable support for onnx runtime." ON)
if (ENABLE_ONNX_RUNTIME)
    add_definitions(-DENABLE_ONNX_RUNTIME)
    include(cmake/onnx_runtime.cmake)
    list(APPEND RUNTIME_INCLUDE_DIRS ${ONNX_RUNTIME_INCLUDE_DIRS})
    list(APPEND RUNTIME_LIBRARIES ${ONNX_RUNTIME_LIBRARIES})
    # 安装时将动态库拷贝到bin和lib目录
    install(FILES ${ONNX_RUNTIME_LIBRARIES} DESTINATION bin)
    install(FILES ${ONNX_RUNTIME_LIBRARIES} DESTINATION third_party/onnxruntime/lib)
    # 安装时将头文件拷贝到third_party/onnxruntime目录
    install(DIRECTORY ${ONNX_RUNTIME_INCLUDE_DIRS} DESTINATION third_party/onnxruntime)
endif ()

option(ENABLE_RKNN_RUNTIME "Enable support for rknn runtime." ON)
if (ENABLE_RKNN_RUNTIME)
    add_definitions(-DENABLE_RKNN_RUNTIME)
    include(cmake/rknn_runtime.cmake)
    list(APPEND RUNTIME_INCLUDE_DIRS ${RKNN_RUNTIME_INCLUDE_DIRS})
    list(APPEND RUNTIME_LIBRARIES ${RKNN_RUNTIME_LIBRARIES})
    # 安装时将动态库拷贝到bin和lib目录
    install(FILES ${RKNN_RUNTIME_LIBRARIES} DESTINATION bin)
    install(FILES ${RKNN_RUNTIME_LIBRARIES} DESTINATION third_party/rknpu2_runtime/lib)
    # 安装时将头文件拷贝到third_party/onnxruntime目录
    install(DIRECTORY ${RKNN_RUNTIME_INCLUDE_DIRS} DESTINATION third_party/rknpu2_runtime)
endif ()

# 安装配置文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION bin)

# 安装Vcpkg第三方库到third_party/vcpkg目录(全为静态库，因此只需要拷贝头文件)
install(DIRECTORY ${VCPKG_INCLUDE_ROOT} DESTINATION third_party/vcpkg)
install(DIRECTORY ${VCPKG_LIBRARY_ROOT} DESTINATION third_party/vcpkg)
install(DIRECTORY ${VCPKG_SHARE_ROOT} DESTINATION third_party/vcpkg)
install(DIRECTORY ${VCPKG_TOOLS_ROOT} DESTINATION third_party/vcpkg)

add_subdirectory(breeze_deploy)

# TODO: 判断编译时是否开启example
add_subdirectory(example)