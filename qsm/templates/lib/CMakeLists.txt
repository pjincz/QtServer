cmake_minimum_required(VERSION 2.8.11)

project(QSM_NAME)

include(cmake/qsm.cmake)

qs_project(
	QSM_NAME
	# add your source and headers files below
	include/QSM_NAME.h
	src/QSM_NAME.cpp

	main.cpp
	)

target_link_libraries(QSM_NAME Qt5::Core Qt5::Network Qt5::Server)
