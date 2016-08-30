#ifndef __ARC_PRIMITIVE_H__
#define __ARC_PRIMITIVE_H__

#ifdef OPENSG_VECTORS

#else

  #include <Windows.h>

  #include <glm\glm.hpp>
  #include <GL\GL.h>

#endif

#include <array>
#include <vector>

// Type of a dynamic tessellated primitive
enum DynamicTessellatedPrimitiveType {
  DYN_TESS_ARC_SEGMENT  = 0,
  DYN_TESS_ARC_TRIANGLE = 1, 
  DYN_TESS_ARC_QUAD     = 2
};

// Vertex of a Dynamic Tessellated Arc Primitive.
struct Vertex {
  glm::vec3 position;
};

// interface for an ArcPrimitive Representation
class DynTessArcPrimitive {
public:
  DynTessArcPrimitive(DynamicTessellatedPrimitiveType type);

  virtual void createBuffer()                                               = 0;
  virtual void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h)  = 0;
  virtual void draw()                                                       = 0;

private:
  DynamicTessellatedPrimitiveType m_type;
};

#endif // __ARC_QUAD_H__