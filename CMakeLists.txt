cmake_minimum_required(VERSION 3.5)

project(cpp_kvs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCES
    src/main.cpp
    src/store/memory/memory.cpp
) # 创建变量 SOURCES, 它包含了所有 cpp 文件。

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
