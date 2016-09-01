#ifndef __ARC_SEGMENT_H__
#define __ARC_SEGMENT_H__

#include "UnifiedMath.h"

// STD
#include <vector>

#ifdef USE_OPENSG

#else

  // Widnows
  #include <windows.h>

  // GL
  #include <GL/glew.h>

#endif

// Dynamic Tessellation
#include "DynTessArcPrimitive.h"

class ArcSegment : public DynTessArcPrimitive
{	
public:
  ArcSegment(
    Vector3D _p1     = Vector3D(1.0f, 0.0f, 0.0f),
    Vector3D _p2     = Vector3D(0.0f, 1.0f, 0.0f),
    Vector3D _center = Vector3D(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs        = 5);

  void createBuffer()                                               override; 
  void updateBuffer(Matrix4x4 mvp, unsigned int w, unsigned int h)  override;
  void draw()                                                       override;

  void setNSegs(const int& nSegs);

//protected:
public:
    Vector3D 	p1;                //    ______       ;
    Vector3D 	p2;                //   /      \     ;
    Vector3D   center;           // p1 ------- p2  ;
  
#ifdef USE_OPENSG

#else

  GLuint buffer = 0;

#endif

  int  nVertices;
  int    nSegs;

};

#endif