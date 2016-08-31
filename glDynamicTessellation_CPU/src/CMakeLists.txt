PROJECT(DynamicTessellation-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring DynamicTessellation Source")



# glob sources from core directories
FILE(GLOB DynamicTessellationSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  glsl/*.frag
  glsl/*.vert
  cl/*.cl
  *.cpp
)

FILE(GLOB DynamicTessellationHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})

# Interpolation method
#ADD_DEFINITIONS(-DUSE_SLERP)
ADD_DEFINITIONS(-DUSE_COMPLEX_METHOD)

ADD_DEFINITIONS(${DynamicTessellationGlobalDefinitions})

INCLUDE_DIRECTORIES(${DynamicTessellationIncludeDirs})
LINK_DIRECTORIES(${GLFW_LIBRARY} ${GLEW_LIBRARY})

MESSAGE("GLEW Library = ${GLEW_LIBRARY}")
MESSAGE("GLFW Library = ${GLFW3_LIBRARY}")

add_executable (DynamicTessellation
  ${DynamicTessellationSources}
  ${DynamicTessellationHeaders}
  )

IF(WIN32)
  TARGET_LINK_LIBRARIES(DynamicTessellation ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(DynamicTessellation ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ENDIF(WIN32)

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(DynamicTessellationSources ${DynamicTessellationHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(DynamicTessellationHeaders ${DynamicTessellationHome} "")