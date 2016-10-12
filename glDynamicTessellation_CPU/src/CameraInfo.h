#ifndef __CAMERA_INFO_H__
#define __CAMERA_INFO_H__

#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Camera information
struct CameraInfo {

  // Camera Type
  enum Type {
    CAMERA_TYPE_ORTHO = 0,
    CAMERA_TYPE_PERSPECTIVE = 1
  } type;

  struct OrthoCamera {
    // TODO incomplete definition
    int   w, h;

    float near_plane_width_world, near_plane_height_world;
    
    // TODO Wrong! The rotation of the camera is not taken into account
    float pixelWidth() const {
      return near_plane_width_world / w;
    }

    // TODO Wrong! The rotation of the camera is not taken into account
    float pixelHeight() const {
      return near_plane_height_world / h;
    }

  };

  struct PerspectiveCamera {
    // TODO Define
  };

  union {
    OrthoCamera       ortho;
    PerspectiveCamera proj;
  };

};

#endif