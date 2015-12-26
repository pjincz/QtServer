cmake_minimum_required(VERSION 2.8.11)

project(QtServer)
set(QtServer_VERSION 0.0.1)

include(cmake/qtbase.cmake)

include_directories(include)

add_subdirectory(src)
#add_subdirectory(test/basic)


