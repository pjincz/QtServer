cmake_minimum_required(VERSION 2.8.11)

add_subdirectory(3rdparty)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#find_package(Qt5Core NO_DEFAULT_PATH)
#find_package(Qt5Network NO_DEFAULT_PATH)

set(CMAKE_CXX_FLAGS "-fPIC")

set(SRCS
	qfiber.cpp
	qhttpservice.cpp
	qhttprequest.cpp
	qhttpresponse.cpp
	qhttpcontext.cpp
	qhttprouter.cpp
	qhttphandler.cpp
	qhttpapplication.cpp
	)
set(HEADERS
	../include/qtserverglobal.h
	../include/qfiber.h

	../include/qhttpcontext.h
	../include/qhttpheaders.h
	../include/qhttprequest.h
	../include/qhttpresponse.h
	../include/qhttpservice.h
	../include/qhttprouter.h
	../include/qhttphandler.h
	../include/qhttpapplication.h
	)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/3rdparty/libcoro")

add_library(QtServer STATIC ${SRCS} ${HEADERS})

target_link_libraries(QtServer Qt5::Core Qt5::Network coro)

