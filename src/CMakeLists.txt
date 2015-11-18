cmake_minimum_required(VERSION 2.8.11)

add_subdirectory(3rdparty)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

set(SRCS
	qpromise.cpp
	qfiber.cpp
	)
set(HEADERS
	../include/qtserverglobal.h
	../include/qpromise.h
	../include/qfiber.h
	)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/3rdparty/libcoro")

add_library(QtServer STATIC ${SRCS} ${HEADERS})

target_link_libraries(QtServer Qt5::Core coro)

