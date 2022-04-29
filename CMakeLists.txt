cmake_minimum_required(VERSION 3.5)
project(cpp_kvs LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(
    store_lib
    src/store/store.h
    src/store/memory/memory.h
    src/store/memory/memory.cpp
    src/store/lsm/memtable.h
    src/store/lsm/memtable.cpp
	src/store/lsm/wal.h
	src/store/lsm/wal.cpp
	src/store/lsm/wal_iterator.h
	src/store/lsm/wal_iterator.cpp
	src/store/lsm/utils.h
	src/store/lsm/utils.cpp
)
target_include_directories(store_lib PRIVATE "${PROJECT_SOURCE_DIR}/src")

# main
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} store_lib)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src")

# testing
add_executable(store_test src/tests/store_test.cpp)
target_link_libraries(store_test store_lib)
target_include_directories(store_test PRIVATE "${PROJECT_SOURCE_DIR}/src")

# test src/store/lsm/memtable
add_executable(lsm_memtable_test src/store/lsm/memtable_test.cpp)
target_link_libraries(lsm_memtable_test store_lib)
target_include_directories(lsm_memtable_test PRIVATE "${PROJECT_SOURCE_DIR}/src")

# test src/store/lsm/wal_iterator
add_executable(lsm_wal_iterator_test src/store/lsm/wal_iterator_test.cpp)
target_link_libraries(lsm_wal_iterator_test store_lib)
target_include_directories(lsm_wal_iterator_test PRIVATE "${PROJECT_SOURCE_DIR}/src")

# test src/store/lsm/wal
add_executable(lsm_wal_test src/store/lsm/wal_test.cpp)
target_link_libraries(lsm_wal_test store_lib)
target_include_directories(lsm_wal_test PRIVATE "${PROJECT_SOURCE_DIR}/src")
enable_testing()

add_test(
    NAME store_test
    COMMAND $<TARGET_FILE:store_test> --success
)

add_test(
    NAME lsm_memtable_test
    COMMAND $<TARGET_FILE:lsm_memtable_test> --success
)

add_test(
	NAME lsm_wal_iterator_test
	COMMAND $<TARGET_FILE:lsm_wal_iterator_test> --success
)

add_test(
	NAME lsm_wal_test
	COMMAND $<TARGET_FILE:lsm_wal_test> --success
)
