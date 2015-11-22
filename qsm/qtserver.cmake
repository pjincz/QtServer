if (QTSERVER_CPP1x)
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

find_package(Qt5Core)
find_package(Qt5Network)

get_filename_component(QTSERVER_QSM ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
get_filename_component(QTSERVER_ROOT ${QTSERVER_QSM} DIRECTORY)

add_library(Qt5::Server STATIC IMPORTED)
set_property(TARGET Qt5::Server 
			 PROPERTY IMPORTED_LOCATION 
			 "${QTSERVER_ROOT}/build/src/libQtServer.a")
set_property(TARGET Qt5::Server 
			 PROPERTY INTERFACE_INCLUDE_DIRECTORIES 
			 "${QTSERVER_ROOT}/include")
set_property(TARGET Qt5::Server 
			 PROPERTY INTERFACE_LINK_LIBRARIES 
			 Qt5::Network "${QTSERVER_ROOT}/build/src/3rdparty/libcoro/libcoro.a")
