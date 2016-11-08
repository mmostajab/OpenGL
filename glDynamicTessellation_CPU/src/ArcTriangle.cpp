#include "ArcTriangle.h"
#include "ArcPrimitiveHelper.h"

// STD
#include <iostream>

using namespace ArcRep;

ArcTriangle::ArcTriangle(
    Vector3Df _p1,
    Vector3Df _p2,
    Vector3Df _p3,
    Vector3Df _center/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs): DynTessArcPrimitive(DYN_TESS_ARC_TRIANGLE) {

  set(_p1, _p2, _p3, _center, _nSegs);
  bufferDrawPrimType = GL_TRIANGLE_FAN;

  }

void ArcTriangle::set(
  Vector3Df _p1,
  Vector3Df _p2,
  Vector3Df _p3,
  Vector3Df _center,
  int _nSegs) {

  p1 = _p1;
  p2 = _p2;
  p3 = _p3;
  center = _center;
  setNSegs(_nSegs);
  createAABB();
}

  void ArcTriangle::createBuffer() {

    
#ifdef USE_OPENSG

#else
    if (buffer > 0) {
      glDeleteBuffers(1, &buffer);
      buffer = 0;
    }
#endif
    

#ifdef USE_OPENSG
  vertices.clear();
  lengths.clear();
  types.clear();
#else
  vertices.clear();
#endif

    Vertex v;
    v.position = p3;

    
#ifdef USE_OPENSG
  vertices.push_back(v.position);
#else
  vertices.push_back(v);
#endif

    Vector3Df c = UnifiedMath::cross(p1-p3, p2-p3);
    bool ccw = c[2] >= 0;

    ArcPrimitiveHelper::produceCurvePoints(p1, p2, center, nSegs, ccw, vertices);

#ifdef USE_OPENSG

  lengths.push_back(static_cast<OSG::UInt32>(vertices.size()));
	types.push_back(GL_TRIANGLE_FAN);

  //createDrawArraysNode(transform, vertices, lengths, types);  
  buffer_size_bytes = vertices.size() * 16;
#else
    
#endif
    nVertices = static_cast<GLint>(vertices.size());
  }

  void ArcTriangle::createAABB()
  {
    AABB aabb;

    UnifiedMath::curveMinMaxPoints(p1, p2, center, aabb.min, aabb.max);

    float r = UnifiedMath::max(UnifiedMath::length(p1 - center), UnifiedMath::length(p2 - center));
    /*aabb.min[0] = center[0] - r;
    aabb.min[1] = center[1] - r;
    aabb.min[2] = center[2] - r;
    
    aabb.max[0] = center[0] + r;
    aabb.max[1] = center[1] + r;
    aabb.max[2] = center[2] + r;*/

    aabb.extend((float*)&p3);
    //aabb.extend((float*)&p1);
    //aabb.extend((float*)&p2);
    //aabb.extend((float*)&center);
    //aabb.enlarge(0.01f);

    setAABB(aabb);
  }

  bool ArcTriangle::updateBuffer(const CameraInfo& camInfo, Matrix4x4f mvp, unsigned int w, unsigned int h) {
    
    if (m_disabled) return false;

    int new_nSegs = 0;
    if(m_tessMethod == TESS_METHOD_CURVE_LENGTH)
      new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, p1, p2, center) / m_tessScale);
    else if (m_tessMethod == TESS_METHOD_FIXED_ALPHA) {
      
      // TODO Consider projection

      // project the pixel
      float pixel_size = 0.f;
      if (camInfo.type == CameraInfo::CAMERA_TYPE_ORTHO) {
        pixel_size = UnifiedMath::max(camInfo.ortho.pixelWidth(), camInfo.ortho.pixelHeight());
      }
      else if (camInfo.type == CameraInfo::CAMERA_TYPE_PERSPECTIVE) {
        std::cout << "Perspective camera is not supported.\n";
      }

      // calculate alpha
      float radius = UnifiedMath::max(UnifiedMath::length(p1 - center), UnifiedMath::length(p2 - center));

      float tri_alpha = 1 - pixel_size / radius;
      if (std::fabs(tri_alpha) >= 1.0f || radius <= m_dropCullingFactor * pixel_size) {
        new_nSegs = 0;
        m_disabled = true;
      }
      else {
        m_disabled = false;
        // calculate the new number of segments
        float alpha = acos(tri_alpha);
        float angle = ArcPrimitiveHelper::angle_between(p1 - center, p2 - center);

        new_nSegs = static_cast<int>(std::ceil(angle / (m_tessScale * m_triangulationAccuracy * alpha) )) ;
      }
    }


//#define UPDATE_ARCS_EVERY_FRAME
#ifndef UPDATE_ARCS_EVERY_FRAME
    if (new_nSegs == nSegs) return false;
#endif

#ifdef USE_OPENSG
#ifndef USE_OPENSG_STANDALONE
    //ifxLog(ifxLogLevel::IFX_NORMAL, "Updating arc triangle.\n"); 
#endif // USE_OPENSG_STANDALONE
#else
    //std::cout << "Number of segments changed from " << nSegs << " to " << new_nSegs << std::endl;
#endif
    setNSegs(new_nSegs);
    return true;
  }

  void ArcTriangle::draw(bool doUpdateGLBuffer) {

    if (m_disabled) return;

    if (doUpdateGLBuffer) updateGLBuffer();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#ifdef USE_OPENSG

#else

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

    glDrawArrays(bufferDrawPrimType, 0, nVertices);

    glDisableVertexAttribArray(0);
#endif

  }

void ArcTriangle::setNSegs(const int& _nSegs)
{
  nSegs = _nSegs;

  createBuffer();
}

int ArcTriangle::getNumGenTriangles() const
{
  if (m_disabled)
    return 0;

  return nSegs;
}