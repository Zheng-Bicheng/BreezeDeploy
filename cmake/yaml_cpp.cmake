find_package(yaml-cpp CONFIG REQUIRED)

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(YAML_CPP_LIBRARIES "yaml-cpp::yaml-cpp")
endif ()
set(YAML_CPP_INCLUDE_DIRS "${YAML_CPP_INCLUDE_DIR}")