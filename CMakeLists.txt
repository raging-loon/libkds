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

    add_compile_options(-Wall -Wextra -Wpedantic)
endif()


include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)


add_executable(libkds_test
    kds_config.c
    kds_test.c
)


add_library(libkds
SHARED
    kds_config.c
)

target_compile_definitions(libkds PRIVATE KDS_DYNLIB)

add_subdirectory(kdscli)
add_subdirectory(modules/pcap)

