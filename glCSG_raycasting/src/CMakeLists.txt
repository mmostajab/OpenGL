PROJECT(CSG-OpenGL-RayCasting-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring CSG-OpenGL-RayCasting Source")



# glob sources from core directories
FILE(GLOB CSG-OpenGL-RayCastingSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
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

FILE(GLOB CSG-OpenGL-RayCastingHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})


ADD_DEFINITIONS(${CSG-OpenGL-RayCastingGlobalDefinitions})

INCLUDE_DIRECTORIES(${CSG-OpenGL-RayCastingIncludeDirs})
LINK_DIRECTORIES(${GLFW_LIBRARY} ${GLEW_LIBRARY})

MESSAGE("GLEW Library = ${GLEW_LIBRARY}")
MESSAGE("GLFW Library = ${GLFW3_LIBRARY}")

add_executable (CSG-OpenGL-RayCasting
  ${CSG-OpenGL-RayCastingSources}
  ${CSG-OpenGL-RayCastingHeaders}
  )

IF(WIN32)
  TARGET_LINK_LIBRARIES(CSG-OpenGL-RayCasting ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(CSG-OpenGL-RayCasting ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ENDIF(WIN32)

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(CSG-OpenGL-RayCastingSources ${CSG-OpenGL-RayCastingHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(CSG-OpenGL-RayCastingHeaders ${CSG-OpenGL-RayCastingHome} "")