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

#ifndef USE_OPENSG_STANDALONE
  #include <base/ifxcore///ifxLog.h>
#endif // USE_OPENSG_STANDALONE

#else

  // Widnows
  //#include <windows.h>

  // GL
  #include <GL/glew.h>

#endif

#include "ArcPrimitiveHelper.h"

namespace ArcRep {

// Type of a dynamic tessellated primitive
enum DynamicTessellatedPrimitiveType {
  DYN_TESS_ARC_SEGMENT  = 0,
  DYN_TESS_ARC_TRIANGLE = 1, 
  DYN_TESS_ARC_QUAD     = 2
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

  virtual void updateGLBuffer();
  virtual bool updateBuffer(const CameraInfo& camInfo, Matrix4x4f mvp, unsigned int w, unsigned int h)  = 0;
  virtual void draw(bool doUpdateGLBuffer)                                                              = 0;
  virtual void drawBoundingBox()                                                                           ;

  //float getTessScale() const;
  //void  setTessScale(float tessScale);
  void  multiplyTessFactor(float multiplier);

  void setDropCullingFactor(float factor);
  void setTrianulationAccuracyFactor(float factor);
  void setAABB(AABB aabb);

  virtual int   getNumGenTriangles() const = 0;
  size_t        getFilledMemBytes() const;
  virtual void  disable() {
    m_disabled = true;
  }
  virtual void  enable() {
    m_disabled = false;
  }

  AABB getAABB() {
    return m_aabb;
  }

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

  std::vector<Vertex> vertices;
  GLuint buffer;
  size_t buffer_size_bytes;
  size_t buffer_filled_bytes;

  GLuint aabb_buffer;

#endif

protected:
  DynamicTessellatedPrimitiveType m_type;
  float                           m_tessScale;
  float                           m_triangulationAccuracy;
  TessellatonMethod               m_tessMethod;

  float                           m_dropCullingFactor;

  AABB                            m_aabb;
  bool                            m_disabled;
  
  virtual void createBuffer() = 0;
  virtual void createAABB() = 0;

};

}

#endif // __ARC_QUAD_H__