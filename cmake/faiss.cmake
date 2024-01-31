if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(FAISS_ROOT "/opt/homebrew/Cellar/faiss/1.7.4")
    set(FAISS_INCLUDE_DIRS "${FAISS_ROOT}/include")
    set(FAISS_LIBRARIES "${FAISS_ROOT}/lib/libfaiss.dylib")
endif()