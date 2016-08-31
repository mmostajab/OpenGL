#ifndef __ARC_TRIANGLE_H__
#define __ARC_TRIANGLE_H__

#ifdef OPENSG_VECTORS

#else

#ifdef _WIN32
  #include <Windows.h>
#endif

  // GL
  #include <gl/glew.h>

  // GLM
  #include <glm\glm.hpp>

#endif

// STD
#include <vector>

#include "DynTessArcPrimitive.h"

extern float mult;

class ArcTriangle : public DynTessArcPrimitive
{
public:
  ArcTriangle(
    glm::vec3 _p1 = glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3 _p2 = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 _p3 = glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3 _center = glm::vec3(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs = 5);

  void createBuffer()                                               override;
  void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h)  override;
  void draw()                                                       override;

  void setNSegs(const int& nSegs);

//protected:
public:
  glm::vec3 	p1;
  glm::vec3 	p2;
  glm::vec3   p3;
  glm::vec3   center;

#ifdef OPENSG_VECTORS

#else

  GLuint buffer = 0;

#endif

  int    nVertices;
  int    nSegs    = -1;
};

#endif