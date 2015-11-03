cmake_minimum_required(VERSION 2.8.11)

project(QtServer)

add_subdirectory(3rdparty)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

set(SRCS
	qpromise.cpp
	qfiber.cpp
	)
set(HEADERS
	qtserverglobal.h
	qpromise.h
	qfiber.h
	)

include_directories("../include" "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/libcoro")
add_executable(testapp test.cpp ${SRCS})

target_link_libraries(testapp Qt5::Core)

