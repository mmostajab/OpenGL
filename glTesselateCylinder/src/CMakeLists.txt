PROJECT(CylinderTesselation-core)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0016 NEW)

MESSAGE(STATUS "Configuring CylinderTesselation Source")



# glob sources from core directories
FILE(GLOB CylinderTesselationSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  glsl/Cylinder/*.shader

  *.h
  *.cpp
  *.c
)

FILE(GLOB CylinderTesselationHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *.h
)

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${GLM_INCLUDE_DIR})


ADD_DEFINITIONS(${CylinderTesselationGlobalDefinitions})

INCLUDE_DIRECTORIES(${CylinderTesselationIncludeDirs})
LINK_DIRECTORIES(${GLFW_LIBRARY} ${GLEW_LIBRARY})

MESSAGE("GLEW Library = ${GLEW_LIBRARY}")
MESSAGE("GLFW Library = ${GLFW3_LIBRARY}")

add_executable (CylinderTesselation
  ${CylinderTesselationSources}
  ${CylinderTesselationHeaders}
  )

IF(WIN32)
  TARGET_LINK_LIBRARIES(CylinderTesselation ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(CylinderTesselation ${Boost_LIBRARIES} ${CGNS_LIBRARIES} ${VTK_LIBRARIES} ${OPENGL_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${ANT_TWEAK_BAR_LIBRARY} ${OPENCL_LIBRARY})
ENDIF(WIN32)

DEFINE_SOURCE_GROUPS_FROM_SUBDIR(CylinderTesselationSources ${CylinderTesselationHome} "")
DEFINE_SOURCE_GROUPS_FROM_SUBDIR(CylinderTesselationHeaders ${CylinderTesselationHome} "")