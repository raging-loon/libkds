project(kdscli VERSION 0.1)

add_executable(kdscli main.c)

target_link_libraries(kdscli PRIVATE libkds)

if(UNIX)
    target_compile_options(kdscli PRIVATE "-ldl")
endif()