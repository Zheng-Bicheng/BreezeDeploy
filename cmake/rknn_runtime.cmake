# RKNN仅支持Arm aarch64平台
if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" OR NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    message(FATAL_ERROR "RKNN_RUNTIME don't supported this system [${CMAKE_SYSTEM_NAME}:${CMAKE_SYSTEM_PROCESSOR}]")
    exit()
endif()

set(RKNN_RUNTIME_ROOT ${CMAKE_SOURCE_DIR}/third_party/rknpu2_runtime)
set(RKNN_RUNTIME_ROOT ${RKNN_RUNTIME_ROOT}/librknn_api)
set(RKNN_RUNTIME_INCLUDE_DIRS "${RKNN_RUNTIME_ROOT}/include")
set(RKNN_RUNTIME_LIBRARIES "${RKNN_RUNTIME_ROOT}/aarch64/librknnrt.so")