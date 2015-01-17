cmake_minimum_required(VERSION 2.8)
project( MBI_src )

file(GLOB_RECURSE CU_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)
file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM CPP_SOURCES main.cpp)

cuda_add_library(cudalib ${CU_SOURCES})
add_library(cpplib ${CPP_SOURCES})

add_executable(exe main.cpp)
target_link_libraries(exe ${GLOG_LIBRARIES} cpplib cudalib)
    