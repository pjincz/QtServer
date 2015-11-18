cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

SET(CMAKE_CXX_FLAGS "-O3")
SET(CMAKE_C_FLAGS "-O3")

find_package(Qt5Core)
find_package(Qt5Network)

set(SRCS
	tcp.cpp
	)

add_executable(aaa ${SRCS})

target_link_libraries(aaa Qt5::Core Qt5::Network)

