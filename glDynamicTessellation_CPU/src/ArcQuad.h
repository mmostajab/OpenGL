#ifndef __ARC_QUAD_H__
#define __ARC_QUAD_H__


#ifdef OPENSG_VECTORS

#else

  // Windows
  #ifdef _WIN32
    #include <Windows.h>
  #endif

  // GL
  #include <gl/glew.h>

  // GLM
  #include <glm\glm.hpp>

#endif

// STD
#include <array>
#include <vector>

#include "DynTessArcPrimitive.h"

extern float mult;

struct ArcQuad : public DynTessArcPrimitive {
public:

  struct HalfArcQuad {
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 center;
  };

  ArcQuad(
    glm::vec3 _p1       = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 _p2       = glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3 _center0  = glm::vec3(0.5f, 0.0f, 0.0f)/*double _alpha0 = glm::pi<double>() / 2.0f*/,
    glm::vec3 _p3       = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 _p4       = glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3 _center1  = glm::vec3(0.5f, 1.0f, 0.0f)/*double _alpha1 = glm::pi<double>() / 2.0f*/,
    int _nSegs          = 5
  );

  void createBuffer();
  void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h);
  void draw();

  void setNSegs(const int & _nSegs);

public:
//protected:
  std::array<HalfArcQuad, 2> halfArcQuad;

#ifdef OPENSG_VECTORS

#else

  GLuint buffer = 0;

#endif

  std::array<size_t, 2>  bufferSizeMaxNumSegs = { 0,  0 };
  int32_t                 nVertices           = -1;
  std::array<int32_t, 2>  nSegs               = { -1, -1 };

};

#endif // __ARC_QUAD_H__