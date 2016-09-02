#ifndef __ARC_TRIANGLE_H__
#define __ARC_TRIANGLE_H__

#include "UnifiedMath.h"

// STD
#include <vector>

#ifdef USE_OPENSG

  #include <OpenSG/OSGNode.h>
  #include <OpenSG/OSGNodePtr.h>
  #include <OpenSG/OSGPerspectiveCamera.h>

  #include "DrawCallNodes.h"

#else

// Widnows
#include <windows.h>

// GL
#include <GL/glew.h>

#endif

#include "DynTessArcPrimitive.h"

class ArcTriangle : public DynTessArcPrimitive
{
public:
  ArcTriangle(
    Vector3Df _p1 = Vector3Df(1.0f, 0.0f, 0.0f),
    Vector3Df _p2 = Vector3Df(0.0f, 1.0f, 0.0f),
    Vector3Df _p3 = Vector3Df(1.0f, 1.0f, 0.0f),
    Vector3Df _center = Vector3Df(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs = 5);

  void createBuffer()                                               override;
  void updateBuffer(Matrix4x4f mvp, unsigned int w, unsigned int h)  override;
  void draw()                                                       override;

  void setNSegs(const int& nSegs);

//protected:
public:
  Vector3Df 	p1;
  Vector3Df 	p2;
  Vector3Df   p3;
  Vector3Df   center;

#ifdef USE_OPENSG

  OSG::NodePtr transform;

#else

  GLuint buffer;

#endif

  int    nVertices;
  int    nSegs;
};

#endif