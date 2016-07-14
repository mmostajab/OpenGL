PROJECT(CSG-OpenGL-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring CSG-OpenGL Source")



# glob sources from core directories
FILE(GLOB CSG-OpenGLSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  glsl/*.frag
  glsl/*.vert

  glsl/box/*.vert
  glsl/box/*.geom
  glsl/box/*.frag

  glsl/directedbox/*.vert
  glsl/directedbox/*.geom
  glsl/directedbox/*.frag

  glsl/cylinder/*.vert
  glsl/cylinder/*.geom
  glsl/cylinder/*.frag
  glsl/cylinder/*.shader

  glsl/polygon/*.vert
  glsl/polygon/*.frag

  cl/*.cl
  *.cpp
  *.c
)

FILE(GLOB CSG-OpenGLHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})


ADD_DEFINITIONS(${CSG-OpenGLGlobalDefinitions})

INCLUDE_DIRECTORIES(${CSG-OpenGLIncludeDirs})
LINK_DIRECTORIES(${GLFW_LIBRARY} ${GLEW_LIBRARY})

MESSAGE("GLEW Library = ${GLEW_LIBRARY}")
MESSAGE("GLFW Library = ${GLFW3_LIBRARY}")

add_executable (CSG-OpenGL
  ${CSG-OpenGLSources}
  ${CSG-OpenGLHeaders}
  )

IF(WIN32)
  TARGET_LINK_LIBRARIES(CSG-OpenGL ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(CSG-OpenGL ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ENDIF(WIN32)

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(CSG-OpenGLSources ${CSG-OpenGLHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(CSG-OpenGLHeaders ${CSG-OpenGLHome} "")