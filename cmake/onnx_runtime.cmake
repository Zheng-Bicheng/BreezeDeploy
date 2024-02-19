set(ONNX_RUNTIME_ROOT "${CMAKE_BINARY_DIR}/third_party/onnxruntime")
function(GetONNXRuntime ONNX_RUNTIME_ROOT)
    set(ONNX_RUNTIME_VERSION "1.16.3")
    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(ONNX_RUNTIME_SYSTEM_NAME "linux")
        if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
            set(ONNX_RUNTIME_SYSTEM_PROCESSOR "x64")
        else ()
            set(ONNX_RUNTIME_SYSTEM_PROCESSOR "aarch64")
        endif ()
    elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        set(ONNX_RUNTIME_SYSTEM_NAME "osx")
        if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
            set(ONNX_RUNTIME_SYSTEM_PROCESSOR "x86_64")
        else ()
            set(ONNX_RUNTIME_SYSTEM_PROCESSOR "arm64")
        endif ()
    else ()
        message(FATAL_ERROR "ONNX_RUNTIME don't supported this system [${CMAKE_SYSTEM_NAME}:${CMAKE_SYSTEM_PROCESSOR}]")
        exit()
    endif ()
    set(ONNX_RUNTIME_FILE_NAME "onnxruntime-${ONNX_RUNTIME_SYSTEM_NAME}-${ONNX_RUNTIME_SYSTEM_PROCESSOR}-${ONNX_RUNTIME_VERSION}")
    set(ONNX_RUNTIME_TGZ_NAME "${ONNX_RUNTIME_FILE_NAME}.tgz")
    set(ONNX_RUNTIME_URL "https://github.com/microsoft/onnxruntime/releases/download/v${ONNX_RUNTIME_VERSION}/${ONNX_RUNTIME_TGZ_NAME}")
    message("Download URL is ${ONNX_RUNTIME_URL}")
    file(DOWNLOAD ${ONNX_RUNTIME_URL} ${ONNX_RUNTIME_TGZ_NAME}
            SHOW_PROGRESS
            #        EXPECTED_HASH SHA256=expected_hash_value
            STATUS download_status)
    list(GET download_status 0 download_result)
    if (download_result)
        message(FATAL_ERROR "Failed to download ONNX Runtime. Error: ${download_result}")
    endif ()

    execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar zxf ${ONNX_RUNTIME_TGZ_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${ONNX_RUNTIME_FILE_NAME} ${ONNX_RUNTIME_ROOT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endfunction()

if (NOT EXISTS ${ONNX_RUNTIME_ROOT})
    GetONNXRuntime(${ONNX_RUNTIME_ROOT})
endif ()

set(CMAKE_CXX_STANDARD 14)
set(ONNX_RUNTIME_INCLUDE_DIRS "${ONNX_RUNTIME_ROOT}/include")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(ONNX_RUNTIME_LIBRARIES "${ONNX_RUNTIME_ROOT}/lib/libonnxruntime.so.1.16.3")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(ONNX_RUNTIME_LIBRARIES "${ONNX_RUNTIME_ROOT}/lib/libonnxruntime.dylib")
else ()
    message(FATAL_ERROR "ONNX_RUNTIME don't supported this system [${CMAKE_SYSTEM_NAME}:${CMAKE_SYSTEM_PROCESSOR}]")
endif ()