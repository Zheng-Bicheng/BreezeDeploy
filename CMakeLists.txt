CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/third_party/vcpkg" CACHE PATH "")
if (${VCPKG_TARGET_TRIPLET} STREQUAL "arm64-cross-compilation")
    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/toolchains/arm64-cross-compilation-toolchain.cmake")
endif ()
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_INSTALLED_ROOT "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
set(VCPKG_INCLUDE_ROOT "${VCPKG_INSTALLED_ROOT}/include")
set(VCPKG_LIBRARY_ROOT "${VCPKG_INSTALLED_ROOT}/lib")

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
    INCLUDE_DIRECTORIES(${RKNN_RUNTIME_INCLUDE_DIRS})
    # 安装时将动态库拷贝到bin和lib目录
    install(FILES ${RKNN_RUNTIME_LIBRARIES} DESTINATION bin)
    install(FILES ${RKNN_RUNTIME_LIBRARIES} DESTINATION third_party/rknpu2_runtime/lib)
    # 安装时将头文件拷贝到third_party/onnxruntime目录
    install(DIRECTORY ${RKNN_RUNTIME_INCLUDE_DIRS} DESTINATION third_party/rknpu2_runtime)
endif ()

# 设置编译对象
set(BREEZE_DEPLOY_SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/breeze_deploy)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${VCPKG_INCLUDE_ROOT})
file(GLOB_RECURSE BREEZE_DEPLOY_HEADS ${BREEZE_DEPLOY_SOURCE_DIRECTORY}/*.h)
file(GLOB_RECURSE BREEZE_DEPLOY_SOURCES ${BREEZE_DEPLOY_SOURCE_DIRECTORY}/*.cpp)
add_library(${CMAKE_PROJECT_NAME} SHARED ${BREEZE_DEPLOY_SOURCES})

find_package(OpenBLAS REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE -Wl,--whole-archive ${FAISS_LIBRARIES} OpenBLAS::OpenBLAS ${SPDLOG_LIBRARIES} ${EIGEN_LIBRARIES} ${YAML_CPP_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC -Wl,--no-whole-archive ${ONNX_RUNTIME_LIBRARIES} ${RKNN_RUNTIME_LIBRARIES} ${OPENCV_LIBRARIES})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        VISIBILITY_INLINES_HIDDEN TRUE
        C_VISIBILITY_PRESET hidden
        POSITION_INDEPENDENT_CODE TRUE
)

# 安装头文件到include目录
install(DIRECTORY ${CMAKE_SOURCE_DIR}/breeze_deploy
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.h"
)
# 安装动态库到lib目录
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION lib)
# 安装配置文件到config目录
install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION .)
# 安装测试程序到bin目录
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION bin)

# 安装Vcpkg第三方库到third_party/vcpkg目录(全为静态库，因此只需要拷贝头文件)
install(DIRECTORY ${VCPKG_INCLUDE_ROOT} DESTINATION third_party/vcpkg)
# 比较特别的是，由于OpenBLAS存在动态库依赖，因此需要将OpenBLAS进行手动的链接
install(DIRECTORY ${VCPKG_LIBRARY_ROOT} DESTINATION third_party/vcpkg)

# TODO: 判断编译时是否开启example
add_subdirectory(example)