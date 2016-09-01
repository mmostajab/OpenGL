#ifndef __ARC_QUAD_H__
#define __ARC_QUAD_H__

#include "UnifiedMath.h"

#ifdef USE_OPENSG

#else

  // Widnows
  #include <windows.h>

  // GL
  #include <GL/glew.h>

#endif

// STD
#include <stdint.h>
#include <array>
#include <vector>

#include "DynTessArcPrimitive.h"

struct ArcQuad : public DynTessArcPrimitive {
public:

  struct HalfArcQuad {
    Vector3D p0;
    Vector3D p1;
    Vector3D center;
  };

  ArcQuad(
    Vector3D _p1       = Vector3D(0.0f, 0.0f, 0.0f),
    Vector3D _p2       = Vector3D(1.0f, 0.0f, 0.0f),
    Vector3D _center0  = Vector3D(0.5f, 0.0f, 0.0f)/*double _alpha0 = glm::pi<double>() / 2.0f*/,
    Vector3D _p3       = Vector3D(0.0f, 1.0f, 0.0f),
    Vector3D _p4       = Vector3D(1.0f, 1.0f, 0.0f),
    Vector3D _center1  = Vector3D(0.5f, 1.0f, 0.0f)/*double _alpha1 = glm::pi<double>() / 2.0f*/,
    int _nSegs          = 5
  );

  void createBuffer();

  void updateBuffer(Matrix4x4 mvp, unsigned int w, unsigned int h);

  void draw();

  void setNSegs(const int & _nSegs);

public:
//protected:
  std::array<HalfArcQuad, 2> halfArcQuad;

#ifdef USE_OPENSG

#else

  GLuint buffer = 0;

#endif

  std::array<size_t, 2>  bufferSizeMaxNumSegs;// = { 0,  0 };
  int32_t                 nVertices;          // = -1;
  std::array<int32_t, 2>  nSegs;              //= { -1, -1 };

};

#endif // __ARC_QUAD_H__