cmake_minimum_required(VERSION 3.10)
project(libkds VERSION 1.0)
project(libkds_test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")
    message("Non submodule build")
    add_compile_options(-Wall -Wextra -Wpedantic)
else()
    message("Building libkds as a submodule")
endif()


include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)


add_executable(libkds_test
    src/kds_config.c
    src/kds_test.c
)


add_library(libkds
SHARED
    src/kds_config.c
)

target_compile_definitions(libkds PRIVATE KDS_DYNLIB)

target_include_directories(libkds PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(kdscli)
add_subdirectory(modules/pcap)

