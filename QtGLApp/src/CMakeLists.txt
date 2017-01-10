PROJECT(NavigationTest-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring NavigationTest Source")

# sources from core directories
set(NavigationTestSources
	Application.cpp
	plydatareader.cpp
	rply.cpp
	main.cpp
	arcball.cpp
	zoom.cpp
	Navigation.cpp
	translationPan.cpp
)

set(NavigationTestHeaders
	Application.h
	plydatareader.h
	rply.h
	AABB.h
	helper.h
	arcball.h
	zoom.h
	Navigation.h
	translationPan.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})

ADD_DEFINITIONS(${NavigationTestGlobalDefinitions})

INCLUDE_DIRECTORIES(${NavigationTestIncludeDirs})

add_executable(NavigationTest
	${NavigationTestSources}
	${NavigationTestHeaders}
)

find_package(Eigen3 REQUIRED)
TARGET_LINK_LIBRARIES(NavigationTest ${OPENGL_LIBRARY} glfw ${GLEW_LIBRARIES})
target_include_directories(NavigationTest PRIVATE ${EIGEN3_INCLUDE_DIR})

add_custom_target(copy-NavigationTest-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/bin/data
    DEPENDS ${MY_TARGET}
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/src/glsl ${CMAKE_BINARY_DIR}/bin/glsl
    DEPENDS ${MY_TARGET}
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
    DEPENDS ${MY_TARGET}
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/src/glsl ${CMAKE_BINARY_DIR}/glsl
    DEPENDS ${MY_TARGET}
	)
add_dependencies(NavigationTest copy-NavigationTest-files)

message("${CMAKE_SOURCE_DIR}")
message("${CMAKE_BINARY_DIR}")

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(NavigationTestSources ${NavigationTestHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(NavigationTestHeaders ${NavigationTestHome} "")
