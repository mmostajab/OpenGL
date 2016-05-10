IF(NOT CommonconfProcessed)

CMAKE_POLICY(SET CMP0005 NEW)
CMAKE_POLICY(SET CMP0011 NEW)

SET(SimplePointsHome ${CMAKE_CURRENT_SOURCE_DIR})
SET(SimplePointsBinaryDir ${CMAKE_BINARY_DIR})

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/Dependencies/boost/boost_1_57_0")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/Dependencies/boost/boost_1_57_0/lib64-msvc-12.0")

MESSAGE(STATUS "SimplePoints Source Directory: ${SimplePointsHome}")
MESSAGE(STATUS "SimplePoints Binary Directory: ${SimplePointsBinaryDir}")

# include macros
INCLUDE(${SimplePointsHome}/cmake/macros.cmake)

# detect compiler and architecture
IF(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013")
    SET(SimplePoints_MSVC2013 TRUE)
    SET(SimplePoints_WIN32 TRUE)
    MESSAGE(STATUS "Visual Studio 2013 Build (32 Bit)")
ELSE()
    MESSAGE(WARNING "Unsupported or unknown generator: ${CMAKE_GENERATOR}")
ENDIF()

SET(Boost_USE_STATIC_LIBS   ON)

# set binary output path
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# common include directories
LIST(APPEND SimplePointsIncludeDirs ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}) 

LIST(APPEND SimplePointsGlobalDefinitions "-DMOBIRFIDVIS_SOURCE_DIR=\"${SimplePointsHome}\"")

# platform-dependent configuration
IF(WIN32)
    LIST(APPEND SimplePointsGlobalDefinitions "-DNOMINMAX" "-D_CRT_SECURE_NO_DEPRECATE")

    # Disable warnings for Microsoft compiler:
    # C4290: C++ exception specification ignored except to indicate a function is
    #        not __declspec(nothrow)
    # C4390: ';' : empty controlled statement found; is this the intent?
    #        occurs when OpenGL error logging macros are disabled
    # C4503: The decorated name was longer than the compiler limit (4096), and was truncated.
    #        Occurs in AutoEvaluatePipeline due to some nested nested map-iterator-map. Could
    #        not be deactivated locally...
    LIST(APPEND SimplePointsGlobalDefinitions /wd4290 /wd4390 /wd4503)

    # prevent error: number of sections exceeded object file format limit
    LIST(APPEND SimplePointsGlobalDefinitions /bigobj)
    
    # allows 32 Bit builds to use more than 2GB RAM (VC++ only)
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LARGEADDRESSAWARE")
    SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
    
    LIST(APPEND SimplePoints_GlobalExternalLibs netapi32 version)
ENDIF(WIN32)

ADD_DEFINITIONS(/WX)

IF(CMAKE_COMPILER_IS_GNUCXX)
    # enable C++11 support in GCC
    LIST(APPEND CMAKE_CXX_FLAGS "-std=c++11")
ENDIF()

IF(STREAM_SURFACE_GENERATOR)

    # OPENGL 
    FIND_PACKAGE(OpenGL REQUIRED)
    FIND_PACKAGE(GLFW3 REQUIRED)
    FIND_PACKAGE(GLEW REQUIRED)
    FIND_PACKAGE(GLM REQUIRED)

    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${GLFW3_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${GLM_INCLUDE_DIR})

ENDIF(STREAM_SURFACE_GENERATOR)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SimplePointsHome}/cmake")

SET(CommonconfProcessed TRUE)

ENDIF(NOT CommonconfProcessed)