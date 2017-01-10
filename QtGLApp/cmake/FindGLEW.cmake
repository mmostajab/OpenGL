#.rst:
# FindGLEW
# --------
#
# Find the OpenGL Extension Wrangler Library (GLEW)
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the :prop_tgt:`IMPORTED` target ``GLEW::GLEW``,
# if GLEW has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   GLEW_INCLUDE_DIRS - include directories for GLEW
#   GLEW_LIBRARIES - libraries to link against GLEW
#   GLEW_FOUND - true if GLEW has been found and can be used

#=============================================================================
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)
find_path(GLEW_INCLUDE_DIR NAMES GL/glew.h PATHS ${OSG_SUPPORT_ROOT}/include)
find_library(GLEW_LIBRARY_RELEASE NAMES GLEW glew32 glew glew32s osgglew PATHS ${OSG_SUPPORT_ROOT} PATH_SUFFIXES lib lib64)
find_library(GLEW_LIBRARY_DEBUG NAMES GLEW glew32D glewD glew32sD osgglewD PATHS ${OSG_SUPPORT_ROOT} PATH_SUFFIXES lib lib64)

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set(GLEW_LIBRARIES optimized ${GLEW_LIBRARY_RELEASE} debug ${GLEW_LIBRARY_DEBUG})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW
                                  REQUIRED_VARS GLEW_INCLUDE_DIR GLEW_LIBRARY_RELEASE GLEW_LIBRARY_DEBUG)

if(GLEW_FOUND AND NOT TARGET GLEW::GLEW)
  add_library(GLEW::GLEW UNKNOWN IMPORTED)
  set_target_properties(GLEW::GLEW PROPERTIES
    IMPORTED_LOCATION_RELEASE "${GLEW_LIBRARY_RELEASE}"
    IMPORTED_LOCATION_DEBUG "${GLEW_LIBRARY_DEBUG}"
    INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}")
endif()

mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)
mark_as_advanced(GLEW_LIBRARIES)
