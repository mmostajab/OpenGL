PROJECT(EyeDomeLighting-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring EyeDomeLighting Source")



# glob sources from core directories
FILE(GLOB EyeDomeLightingSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  glsl/*.frag
  glsl/*.vert
  cl/*.cl
  *.cpp
)

FILE(GLOB EyeDomeLightingHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})


ADD_DEFINITIONS(${EyeDomeLightingGlobalDefinitions})

INCLUDE_DIRECTORIES(${EyeDomeLightingIncludeDirs})
LINK_DIRECTORIES(${GLFW_LIBRARY} ${GLEW_LIBRARY})

MESSAGE("GLEW Library = ${GLEW_LIBRARY}")
MESSAGE("GLFW Library = ${GLFW3_LIBRARY}")

add_executable (EyeDomeLighting
  ${EyeDomeLightingSources}
  ${EyeDomeLightingHeaders}
  )

IF(WIN32)
  TARGET_LINK_LIBRARIES(EyeDomeLighting ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ASSIMP_LIBRARY_RELEASE})
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(EyeDomeLighting  ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ENDIF(WIN32)

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(EyeDomeLightingSources ${EyeDomeLightingHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(EyeDomeLightingHeaders ${EyeDomeLightingHome} "")