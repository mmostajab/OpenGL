#ifndef __ARC_PRIMITIVE_H__
#define __ARC_PRIMITIVE_H__

#include <glm\glm.hpp>
#include <GL\GL.h>

#include <array>
#include <vector>

enum DynamicTessellatedPrimitiveType {
  DYN_TESS_ARC_SEGMENT  = 0,
  DYN_TESS_ARC_TRIANGLE = 1, 
  DYN_TESS_ARC_QUAD     = 2
};

// interface for an ArcPrimitive Representation
class DynTessArcPrimitive {

  // Vertex of a Dynamic Tessellated Arc Primitive.
  struct Vertex {
    glm::vec3 position;
  };

public:
  virtual void createBuffer()                                               = 0;
  virtual void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h)  = 0;
  virtual void draw()                                                       = 0;
};

#endif // __ARC_QUAD_H__