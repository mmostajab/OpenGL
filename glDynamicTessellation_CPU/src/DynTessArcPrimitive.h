#ifndef __ARC_PRIMITIVE_H__
#define __ARC_PRIMITIVE_H__

#include "UnifiedMath.h"
#include "CameraInfo.h"

#include <array>
#include <vector>

#ifdef USE_OPENSG
  #include <OpenSG/OSGNode.h>
  #include <OpenSG/OSGNodePtr.h>
  #include <OpenSG/OSGPerspectiveCamera.h>
  #include <OpenSG/OSGComponentTransform.h>
  #include <OpenSG/OSGSwitch.h>

  #include "DrawCallNodes.h"

  #include <base/ifxcore/ifxLog.h>

#else

  // Widnows
  //#include <windows.h>

  // GL
  #include <GL/glew.h>

#endif

namespace ArcRep {

// Type of a dynamic tessellated primitive
enum DynamicTessellatedPrimitiveType {
  DYN_TESS_ARC_SEGMENT  = 0,
  DYN_TESS_ARC_TRIANGLE = 1, 
  DYN_TESS_ARC_QUAD     = 2
};

// Vertex of a Dynamic Tessellated Arc Primitive.
struct Vertex {
  Vector3Df position;
};

enum TessellatonMethod {
  TESS_METHOD_CURVE_LENGTH             = 0,
  TESS_METHOD_FIXED_ALPHA              = 1,
  TESS_METHOD_CIRCULAR_CURVE_SUBDIV    = 2
};

// interface for an ArcPrimitive Representation
class DynTessArcPrimitive {
public:
  DynTessArcPrimitive(DynamicTessellatedPrimitiveType type);

  virtual bool updateBuffer(const CameraInfo& camInfo, Matrix4x4f mvp, unsigned int w, unsigned int h)  = 0;
  virtual void draw()                                                                                   = 0;

  float getTessScale() const;
  void  setTessScale(float tessScale);
  void  multiplyTessFactor(float multiplier);

  virtual int   getNumGenTriangles() const = 0;

//protected:
public:

#ifdef USE_OPENSG
public:
  const std::vector<OSG::Pnt3f>&  getVertices() const;
  const std::vector<OSG::UInt32>& getLengths()  const;
  const std::vector<OSG::UInt8>&  getTypes()    const;

protected:
  /*OSG::NodePtr transform;
  OSG::NodePtr switchNode;*/

  std::vector<OSG::Pnt3f> vertices;
  std::vector<OSG::UInt32> lengths;
	std::vector<OSG::UInt8>  types;

#else

  GLuint buffer;

#endif

protected:
  DynamicTessellatedPrimitiveType m_type;
  float                           m_tessScale;
  float                           m_triangulationAccuracy;
  TessellatonMethod               m_tessMethod;

  float                           m_dropCullingFactor;

  virtual void createBuffer() = 0;

};

}

#endif // __ARC_QUAD_H__