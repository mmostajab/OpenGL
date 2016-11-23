#ifndef __ARC_TRIANGLE_H__
#define __ARC_TRIANGLE_H__

#include "UnifiedMath.h"

// STD
#include <vector>

#include "DynTessArcPrimitive.h"

namespace ArcRep {

class ArcTriangle : public DynTessArcPrimitive
{
public:
  ArcTriangle(
    Vector3Df _p1 = Vector3Df(1.0f, 0.0f, 0.0f),
    Vector3Df _p2 = Vector3Df(0.0f, 1.0f, 0.0f),
    Vector3Df _p3 = Vector3Df(1.0f, 1.0f, 0.0f),
    Vector3Df _center = Vector3Df(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs = 10);

  void set(
    Vector3Df _p1,
    Vector3Df _p2,
    Vector3Df _p3,
    Vector3Df _center,
    int _nSegs);

  
  bool updateBuffer(const CameraInfo& camInfo, Matrix4x4f mvp, unsigned int w, unsigned int h)  override;
  void draw(bool doUpdateGLBuffer)                                                        override;

  void setNSegs(const int& nSegs);

  int  getNumGenTriangles() const;

protected:

  void createBuffer() override;
  void createAABB() override;

  Vector3Df 	p1;
  Vector3Df 	p2;
  Vector3Df   p3;
  Vector3Df   center;

  int    nVertices;
  int    nSegs;

  
};

}

#endif