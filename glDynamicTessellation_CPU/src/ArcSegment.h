#ifndef __ARC_SEGMENT_H__
#define __ARC_SEGMENT_H__

#include "UnifiedMath.h"

// STD
#include <vector>

// Dynamic Tessellation
#include "DynTessArcPrimitive.h"

namespace ArcRep {

class ArcSegment : public DynTessArcPrimitive
{	
public:
  ArcSegment(
    Vector3Df _p1     = Vector3Df(1.0f, 0.0f, 0.0f),
    Vector3Df _p2     = Vector3Df(0.0f, 1.0f, 0.0f),
    Vector3Df _center = Vector3Df(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs        = 100);

  void set(
    Vector3Df _p1,
    Vector3Df _p2,
    Vector3Df _center,
    int _nSegs);

  
  bool updateBuffer(const CameraInfo& camInfo, Matrix4x4f mvp, unsigned int w, unsigned int h)  override;
  void draw()                                                       override;

  void setNSegs(const int& nSegs);

  int  getNumGenTriangles() const;

protected:
//public:
    Vector3Df 	p1;                //    ______       ;
    Vector3Df 	p2;                //   /      \     ;
    Vector3Df   center;           // p1 ------- p2  ;
  
  int  nVertices;
  int    nSegs;

  void createBuffer()                                               override;

};

}
#endif