cmake_minimum_required(VERSION 2.8.11)

project(QtServer)
set(QtServer_VERSION 0.0.1)

include(cmake/qtbase.cmake)

#set(CMAKE_CXX_FLAGS "-std=c++11")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")

include_directories(include)

add_subdirectory(src)
#add_subdirectory(test/basic)


