cmake_minimum_required(VERSION 3.5)

project(
    cpp_kvs
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(
    store_lib
    src/store/store.h
    src/store/memory/memory.h
    src/store/memory/memory.cpp
)

target_include_directories(
    store_lib
    PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} store_lib)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

# testing
add_executable(store_test src/tests/store_test.cpp)

target_link_libraries(store_test store_lib)

target_include_directories(
    store_test
    PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

enable_testing()

add_test(
    NAME store_test
    COMMAND $<TARGET_FILE:store_test> --success
)

