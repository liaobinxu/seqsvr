cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CUSTOME_CXX_FLAGS "-Wall -D_REENTRANT -D_GNU_SOURCE -D_XOPEN_SOURCE -fPIC -m64")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g2  ${CUSTOME_CXX_FLAGS} ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -o2  ${CUSTOME_CXX_FLAGS}")

#add_compile_options(-std=c++11)
add_definitions(-std=c++11 -pipe)
add_definitions(-DWITH_GPERFTOOLS)

find_package(baserpc REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(seqsvr ${SOURCE_FILES})

add_subdirectory("comm")
add_subdirectory("allocsvr")
add_subdirectory("storesvr")

add_subdirectory("allocsvr_tests")
add_subdirectory("comm_tests")