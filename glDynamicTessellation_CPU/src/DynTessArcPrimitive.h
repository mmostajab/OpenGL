#ifndef __ARC_PRIMITIVE_H__
#define __ARC_PRIMITIVE_H__

#include "UnifiedMath.h"

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
  Vector3D position;
};

// interface for an ArcPrimitive Representation
class DynTessArcPrimitive {
public:
  DynTessArcPrimitive(DynamicTessellatedPrimitiveType type);

  virtual void createBuffer()                                               = 0;
  virtual void updateBuffer(Matrix4x4 mvp, unsigned int w, unsigned int h)  = 0;
  virtual void draw()                                                       = 0;

  float getTessScale() const;
  void  setTessScale(float tessScale);
  void  multiplyTessFactor(float multiplier);

protected:
  DynamicTessellatedPrimitiveType m_type;
  float                           m_tessScale;
};

#endif // __ARC_QUAD_H__