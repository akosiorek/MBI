cmake_minimum_required(VERSION 2.8)
project( MBI )

#    Set CMake Install Path
set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "default install path" FORCE )

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_COMPILER_FLAGS})
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -gencode arch=compute_20,code=sm_21)
add_definitions ("-Wall") 

#	CMake module path for custom module finding
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules)

find_package(Glog)
include_directories(${GLOG_INCLUDE_DIRS})

find_package(CUDA 5.5 REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
 