cmake_minimum_required(VERSION 2.8.11)

project(QtServer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

add_subdirectory(3rdparty)

add_executable(testapp test.cpp)

target_link_libraries(testapp Qt5::Core)

