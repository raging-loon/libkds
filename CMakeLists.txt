cmake_minimum_required(VERSION 3.10)
project(libkds VERSION 1.0)

set(CMAKE_C_STANDARD 11)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")

    add_compile_options(-Wall -Wextra -Wpedantic)
endif()


include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)
add_subdirectory(kdscli)
add_subdirectory(modules/pcap)
