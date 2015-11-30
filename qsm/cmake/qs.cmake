if (QTSERVER_CPP1x)
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

include(${QS_PATH}/cmake/qtbase.cmake)

add_library(Qt5::Server STATIC IMPORTED)
set_property(TARGET Qt5::Server PROPERTY IMPORTED_LOCATION 
	"${QS_PATH}/build/src/libQtServer.a")
set_property(TARGET Qt5::Server PROPERTY INTERFACE_INCLUDE_DIRECTORIES 
	"${QS_PATH}/include")
set_property(TARGET Qt5::Server PROPERTY INTERFACE_LINK_LIBRARIES 
	Qt5::Network Qt5::Core "${QS_PATH}/build/src/3rdparty/libcoro/libcoro.a")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(CMAKE_EXE_LINKER_FLAGS "-static")
