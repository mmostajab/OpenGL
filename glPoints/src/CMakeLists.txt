PROJECT(SimplePoints-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring SimplePoints Source")



# glob sources from core directories
FILE(GLOB SimplePointsSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  glsl/*.frag
  glsl/*.vert
  cl/*.cl
  *.cpp
)

FILE(GLOB SimplePointsHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})


ADD_DEFINITIONS(${SimplePointsGlobalDefinitions})

INCLUDE_DIRECTORIES(${SimplePointsIncludeDirs})
LINK_DIRECTORIES(${GLFW_LIBRARY} ${GLEW_LIBRARY})

MESSAGE("GLEW Library = ${GLEW_LIBRARY}")
MESSAGE("GLFW Library = ${GLFW3_LIBRARY}")

add_executable (SimplePoints
  ${SimplePointsSources}
  ${SimplePointsHeaders}
  )

IF(WIN32)
  TARGET_LINK_LIBRARIES(SimplePoints ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(SimplePoints ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ENDIF(WIN32)

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(SimplePointsSources ${SimplePointsHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(SimplePointsHeaders ${SimplePointsHome} "")