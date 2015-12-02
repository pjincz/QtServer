get_filename_component(QTSERVER_CMAKE ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
get_filename_component(QTSERVER_ROOT ${QTSERVER_CMAKE} DIRECTORY)
set(QTBASE_ROOT "${QTSERVER_ROOT}/qtbase")
set(QT_VERSION_MAJOR "5")

if (NOT TARGET Qt5::qmake)
    add_executable(Qt5::qmake IMPORTED)

    set_target_properties(Qt5::qmake PROPERTIES
		IMPORTED_LOCATION "${QTBASE_ROOT}/bin/qmake"
    )
endif()

if (NOT TARGET Qt5::moc)
    add_executable(Qt5::moc IMPORTED)

    set_target_properties(Qt5::moc PROPERTIES
		IMPORTED_LOCATION "${QTBASE_ROOT}/bin/moc"
    )
    # For CMake automoc feature
    get_target_property(QT_MOC_EXECUTABLE Qt5::moc LOCATION)
endif()

if (NOT TARGET Qt5::rcc)
    add_executable(Qt5::rcc IMPORTED)

    set_target_properties(Qt5::rcc PROPERTIES
		IMPORTED_LOCATION "${QTBASE_ROOT}/bin/rcc"
    )
endif()

if (NOT TARGET qtpcre)
	add_library(qtpcre STATIC IMPORTED)
	set_target_properties(qtpcre PROPERTIES
		IMPORTED_LOCATION "${QTBASE_ROOT}/lib/libqtpcre.a")
endif()

if (NOT TARGET Qt5::Core)
	add_library(Qt5::Core STATIC IMPORTED)
	set_target_properties(Qt5::Core PROPERTIES
		IMPORTED_LOCATION "${QTBASE_ROOT}/lib/libQt5Core.a")
	set_property(TARGET Qt5::Core APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
		"${QTBASE_ROOT}/include" "${QTBASE_ROOT}/include/QtCore")
endif()

if (NOT TARGET Qt5::Network)
	add_library(Qt5::Network STATIC IMPORTED)
	set_target_properties(Qt5::Network PROPERTIES
		IMPORTED_LOCATION "${QTBASE_ROOT}/lib/libQt5Network.a")
	set_property(TARGET Qt5::Network APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
		"${QTBASE_ROOT}/include" "${QTBASE_ROOT}/include/QtNetwork")
endif()

set(Qt5Core_QMAKE_EXECUTABLE Qt5::qmake)
set(Qt5Core_MOC_EXECUTABLE Qt5::moc)
set(Qt5Core_RCC_EXECUTABLE Qt5::rcc)

function(set_link_dep_from_prl TARGET)
	string(REPLACE "::" "" X "${TARGET}")
	set(PRL_FILE "${QTBASE_ROOT}/lib/lib${X}.prl")
	file(STRINGS ${PRL_FILE} LINES)
	foreach(L ${LINES})
		if (L MATCHES "^QMAKE_PRL_LIBS ")
			string(REGEX REPLACE "^.*= *" "" L "${L}")
			string(REGEX REPLACE "-L[^ ]* *" "" L "${L}")
			string(REPLACE "-l" "" L "${L}")
			if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
				# hack, add dependency to Qt5Core, I don't known why they are leaked
				set(L "pthread ${L}")
				string(REPLACE "execinfo" "execinfo elf" L "${L}")
			endif()
			string(REPLACE "Qt5" "Qt5::" L "${L}")
			string(REPLACE " " ";" L ${L})
			set_property(TARGET ${TARGET} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${L})
		endif()
	endforeach()
endfunction()

set_link_dep_from_prl(Qt5::Core)
set_link_dep_from_prl(Qt5::Network)

