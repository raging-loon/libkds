project(kdscli VERSION 0.1)

add_executable(kdscli main.c)

if(UNIX)
    target_compile_options(kdscli PRIVATE "-ldl")
endif()