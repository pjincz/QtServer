if (NOT QS_PATH)
	message(FATAL_ERROR "QS_PATH not specified!")
endif()

include("${QS_PATH}/qsm/cmake/qs.cmake")
