if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(OpenMP REQUIRED)
    set(FAISS_INCLUDE_DIRS "/usr/include;${OpenMP_CXX_FLAGS}")
    set(FAISS_LIBRARIES "/usr/local/lib/libfaiss.a;${OpenMP_CXX_LIBRARIES}")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(FAISS_ROOT "/opt/homebrew/Cellar/faiss/1.7.4")
    set(FAISS_INCLUDE_DIRS "${FAISS_ROOT}/include")
    set(FAISS_LIBRARIES "${FAISS_ROOT}/lib/libfaiss.dylib")
else ()
    message(FATAL_ERROR "FAISS don't supported this system [${CMAKE_SYSTEM_NAME}:${CMAKE_SYSTEM_PROCESSOR}]")
endif ()