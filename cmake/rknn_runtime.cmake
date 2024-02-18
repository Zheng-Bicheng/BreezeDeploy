# RKNN仅支持Arm aarch64平台
if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" OR NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    message(FATAL_ERROR "ONNX_RUNTIME don't supported this system [${CMAKE_SYSTEM_NAME}:${CMAKE_SYSTEM_PROCESSOR}]")
endif()

set(RKNN_RUNTIME_ROOT "${CMAKE_BINARY_DIR}/third_party/rknpu2_runtime")
function(GetRKNNRuntime RKNN_RUNTIME_ROOT)
    # https://gitee.com/BreezeDeploy/rknpu2_runtime/archive/refs/tags/v1.6.1b10.tar.gz
    set(RKNN_RUNTIME_VERSION "1.6.1b10")
    set(RKNN_RUNTIME_TGZ_NAME "v${RKNN_RUNTIME_VERSION}.tar.gz")
    set(RKNN_RUNTIME_URL "https://gitee.com/BreezeDeploy/rknpu2_runtime/archive/refs/tags/${RKNN_RUNTIME_TGZ_NAME}")
    message("Download URL is ${ONNX_RUNTIME_URL}")
    file(DOWNLOAD ${RKNN_RUNTIME_URL} ${RKNN_RUNTIME_TGZ_NAME}
            SHOW_PROGRESS
            #        EXPECTED_HASH SHA256=expected_hash_value
            STATUS download_status)
    list(GET download_status 0 download_result)
    if (download_result)
        message(FATAL_ERROR "Failed to download ONNX Runtime. Error: ${download_result}")
    endif ()

    execute_process(COMMAND ${CMAKE_COMMAND} -E tar zxf ${RKNN_RUNTIME_TGZ_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${ONNX_RUNTIME_FILE_NAME} ${ONNX_RUNTIME_ROOT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endfunction()

if (NOT EXISTS ${RKNN_RUNTIME_ROOT})
    GetRKNNRuntime(${RKNN_RUNTIME_ROOT})
endif ()

set(RKNN_RUNTIME_ROOT ${RKNN_RUNTIME_ROOT}/runtime/Linux/librknn_api)
set(RKNN_RUNTIME_INCLUDE_DIRS "${ONNX_RUNTIME_ROOT}/include")
set(RKNN_RUNTIME_LIBRARIES "${ONNX_RUNTIME_ROOT}/aarch64/librknnrt.so")