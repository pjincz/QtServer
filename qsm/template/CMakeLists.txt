cmake_minimum_required(VERSION 2.8.11)

project(QSM_NAME)

set(QTSERVER_CPP1x on)
include(cmake/qsm.cmake)

set(SRCS
	# add your source and headers files here
	main.cpp
	)

add_executable(app ${SRCS})
target_link_libraries(app Qt5::Core Qt5::Network Qt5::Server)
set_property(TARGET app PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
