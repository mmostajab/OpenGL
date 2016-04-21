#
# StreamLineGenerator - Yet another visualization framework
# CMake file inspired by Voreen
#

PROJECT(SimpleGLApp)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11 FATAL_ERROR)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
SET(CMAKE_BUILD_TYPE DEBUG)
SET(STREAM_SURFACE_GENERATOR ON)
#
# Further Build Configurations
#
include(cmake/commonconf.cmake)

MESSAGE(STATUS "--------------------------------------------------------------------------------")

ADD_SUBDIRECTORY(src)

#
# Copy External DLLs
#
IF(WIN32)  

#	get_target_property(QtCore_location Qt5::Core DEBUG_LOCATION)
#	get_target_property(QtGui_location Qt5::Gui DEBUG_LOCATION)
#	get_target_property(QtWidgets_location Qt5::Widgets DEBUG_LOCATION)

#	file(GLOB QtNeededDLLs "${_qt5Core_install_prefix}/bin/icu*.dll"
#			"${_qt5Core_install_prefix}/bin/libgles*d.dll"
#			"${_qt5Core_install_prefix}/bin/libegld.dll")
	
#    LIST(APPEND RFID_MobiVisExternalDllsDebug  
#	${IRRLICHT_DLL} 
#	${QtCore_location} ${QtGui_location} ${QtWidgets_location}
#	${QtNeededDLLs}
#	)
#    LIST(APPEND RFID_MobiVisExternalDllsRelease ${IRRLICHT_DLL})

#    LIST(REMOVE_DUPLICATES RFID_MobiVisExternalDllsDebug)
#    LIST(REMOVE_DUPLICATES RFID_MobiVisExternalDllsRelease)
	
#    COPY_EXTERNAL_DLLS(RFID_MobiVisExternalDllsDebug RFID_MobiVisExternalDllsRelease false)


ELSE()
	# set compiler for using of default gcc via macport (gcc 4.8)
	# set(CMAKE_CXX_COMPILER "clang++")
	# set(CMAKE_C_COMPILER "clang")

	# set the release mode for build type
	SET (CMAKE_BUILD_TYPE "Release")

	# set c++ compiler flags -on warning and using of new c++ strandard library ISO 2011 (-std=c++0x)
	SET (CMAKE_CXX_FLAGS "-Wall -Wno-deprecated -fopenmp")

	# message for set the compiler and its flags and type of build(release)
	message ("user define: CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
	message ("user define: CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")
	message ("user define: CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")

	IF(CMAKE_BUILD_TYPE STREQUAL Debug)
		SET (BUILD_PREFIX "-d")
	ELSE()
		SET (BUILD_PREFIX "")
	ENDIF()
ENDIF()