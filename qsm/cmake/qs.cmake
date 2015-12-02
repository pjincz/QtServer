if(QTSERVER_CPP1x)
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


macro(qs_project prj)
	set(_HEADERS)
	set(_SRCS)
	set(_REST)

	foreach(X ${ARGN})
		if(X MATCHES "^include/")
			list(APPEND _HEADERS ${X})
		elseif(X MATCHES "^src/")
			list(APPEND _SRCS ${X})
		else()
			list(APPEND _REST ${X})
		endif()
	endforeach()

	if(${_SRCS})
		add_library(${prj} STATIC ${_HEADERS} ${_SRCS})
		set_property(TARGET ${prj} APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")
	else()
		add_library(${prj} STATIC ${QS_PATH}/qsm/cmake/empty.c)
	endif()

	if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
		add_executable(app ${_REST})
		target_link_libraries(app ${prj})
		set_property(TARGET app PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
		set_property(TARGET app APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")
	endif()
endmacro()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	include("${CMAKE_SOURCE_DIR}/qs_modules/CMakeLists.txt")
endif()
