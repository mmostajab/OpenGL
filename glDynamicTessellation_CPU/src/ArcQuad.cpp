#include "ArcQuad.h"

// STD
#include <iostream>

#include "ArcPrimitiveHelper.h"

using namespace ArcRep;

ArcQuad::ArcQuad(Vector3Df _p1, Vector3Df _p2, Vector3Df _center0, Vector3Df _p3, Vector3Df _p4, Vector3Df _center1, int _nSegs): 
  DynTessArcPrimitive(DYN_TESS_ARC_QUAD)
{
  set(_p1, _p2, _center0, _p3, _p4, _center1, _nSegs);
  bufferDrawPrimType = GL_TRIANGLE_STRIP;
}

void ArcQuad::set(Vector3Df _p1, Vector3Df _p2, Vector3Df _center0, Vector3Df _p3, Vector3Df _p4, Vector3Df _center1, int _nSegs)
{
  halfArcQuad[0].p0 = _p1;
  halfArcQuad[0].p1 = _p2;
  halfArcQuad[0].center = _center0;

  halfArcQuad[1].p0 = _p4;
  halfArcQuad[1].p1 = _p3;
  halfArcQuad[1].center = _center1;

  std::array<int32_t, 2> NumberOfSegments = { _nSegs, _nSegs };
  setNSegs(NumberOfSegments);
  createAABB();
}

void ArcQuad::createBuffer() {

#ifdef USE_OPENSG
  vertices.clear();
  lengths.clear();
  types.clear();
#else
  vertices.clear();
#endif

  std::vector<Vertex> curve_points[2];

  ArcPrimitiveHelper::produceCurvePoints(halfArcQuad[0].p0, halfArcQuad[0].p1, halfArcQuad[0].center, nSegs[0], false, curve_points[0]);
  ArcPrimitiveHelper::produceCurvePoints(halfArcQuad[1].p0, halfArcQuad[1].p1, halfArcQuad[1].center, nSegs[0], false, curve_points[1]);

  Vertex v;

  if(curve_points[0].size() == curve_points[1].size()){

    bufferDrawPrimType = GL_TRIANGLE_STRIP;

    for(size_t i = 0; i < curve_points[0].size(); i++){

#ifdef USE_OPENSG
      v = curve_points[0][i];
      vertices.push_back(v);

      v = curve_points[1][i];
      vertices.push_back(v);
#else
      v.position = curve_points[0][i].position;
      vertices.push_back(v);
      
      v.position = curve_points[1][i].position;
      vertices.push_back(v);
#endif

    }

#ifdef USE_OPENSG
    lengths.push_back(static_cast<OSG::UInt32>(vertices.size()));
    types.push_back(GL_TRIANGLE_STRIP);
#endif

  } else {
    bufferDrawPrimType = GL_TRIANGLES;
    /*
    size_t i = 0, j = 0;
    while (i+1 < curve_points[0].size() && j+1 < curve_points[1].size()) {

      Vertex v0, v1;
      v0.position = curve_points[0][i];
      v1.position = curve_points[1][j];

      float length_left  = UnifiedMath::length(curve_points[0][i + 1] - curve_points[1][j]);
      float length_right = UnifiedMath::length(curve_points[0][i] - curve_points[1][j + 1]);
      if (length_left <= length_right) {
        v.position = curve_points[0][i + 1];
        i++;
      } else {
        v.position = curve_points[1][j + 1];
        j++;
      }
#ifdef USE_OPENSG
      vertices.push_back(OSG::Pnt3f(v0.position));
      vertices.push_back(OSG::Pnt3f(v1.position));
      vertices.push_back(OSG::Pnt3f(v.position));
#else
      vertices.push_back(v0);
      vertices.push_back(v1);
      vertices.push_back(v);
#endif
    }

    for (; i+1 < curve_points[0].size(); i++) {
      Vertex v0, v1;
      v0.position = curve_points[0][i];
      v1.position = curve_points[1][j];
      v.position = curve_points[0][i + 1];

#ifdef USE_OPENSG
      vertices.push_back(OSG::Pnt3f(v0.position));
      vertices.push_back(OSG::Pnt3f(v1.position));
      vertices.push_back(OSG::Pnt3f(v.position));
#else
      vertices.push_back(v0);
      vertices.push_back(v1);
      vertices.push_back(v);
#endif
    }

    for (; j + 1 < curve_points[1].size(); j++) {
      Vertex v0, v1;
      v0.position = curve_points[0][i];
      v1.position = curve_points[1][j];
      v.position = curve_points[1][j + 1];
#ifdef USE_OPENSG
      vertices.push_back(OSG::Pnt3f(v0.position));
      vertices.push_back(OSG::Pnt3f(v1.position));
      vertices.push_back(OSG::Pnt3f(v.position));
#else
      vertices.push_back(v0);
      vertices.push_back(v1);
      vertices.push_back(v);
#endif

    }

#ifdef USE_OPENSG
    lengths.push_back(static_cast<OSG::UInt32>(vertices.size()));
    types.push_back(GL_TRIANGLES);
#endif
*/
  }

  size_t required_buffer_size = vertices.size() * sizeof(Vertex);
  buffer_filled_bytes = required_buffer_size;
  nVertices = static_cast<GLint>(vertices.size());

#ifdef USE_OPENSG
  //createDrawArraysNode(transform, vertices, lengths, types);  
#else
  
  /*if (buffer <= 0 || required_buffer_size > buffer_size_bytes) {
    if (buffer > 0) glDeleteBuffers(1, &buffer);
    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, required_buffer_size, vertices.data(), GL_STATIC_DRAW);
    buffer_size_bytes = required_buffer_size;
  }
  else {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, required_buffer_size, vertices.data(), GL_STATIC_DRAW);
  }*/
  
#endif
           
  }

  void ArcRep::ArcQuad::createAABB()
  {
    AABB aabb0, aabb1;

    UnifiedMath::curveMinMaxPoints(halfArcQuad[0].p0, halfArcQuad[0].p1, halfArcQuad[0].center, aabb0.min, aabb0.max);
    UnifiedMath::curveMinMaxPoints(halfArcQuad[1].p0, halfArcQuad[1].p1, halfArcQuad[1].center, aabb1.min, aabb1.max);
    aabb0.extend((float*)&aabb1.min);
    aabb0.extend((float*)&aabb1.max);
    aabb0.enlarge(0.0001f);

    setAABB(aabb0);
  }

bool ArcQuad::updateBuffer(const CameraInfo& camInfo, Matrix4x4f mvp, unsigned int w, unsigned int h) {

  if (m_disabled) return false;

  std::array<int32_t, 2> new_nSegs = { 0, 0 };

  for (size_t i = 0; i < halfArcQuad.size(); i++) {
    if (m_tessMethod == TESS_METHOD_CURVE_LENGTH)
      new_nSegs[i] = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, halfArcQuad[i].p0, halfArcQuad[i].p1, halfArcQuad[i].center) / m_tessScale);
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
      float radius = UnifiedMath::max(UnifiedMath::length(halfArcQuad[i].p0 - halfArcQuad[i].center), UnifiedMath::length(halfArcQuad[i].p1 - halfArcQuad[i].center));

      float tri_alpha = 1 - pixel_size / radius;
      if (std::fabs(tri_alpha) >= 1.0f || radius <= m_dropCullingFactor * pixel_size) {
        new_nSegs[i] = 0;
        m_disabled = true;
      }
      else {
        m_disabled = false;
        // calculate the new number of segments
        float alpha = acos(tri_alpha);
        float angle = ArcPrimitiveHelper::angle_between(halfArcQuad[i].p0 - halfArcQuad[i].center, halfArcQuad[i].p1 - halfArcQuad[i].center);

        new_nSegs[i] = static_cast<int>(std::ceil(angle / (m_tessScale * m_triangulationAccuracy * alpha)));
      }
    }

  }

  new_nSegs[0] = new_nSegs[1] = std::max(new_nSegs[0], new_nSegs[1]);

// if the buffer does not need to change
  if (!m_updateEveryFrame && nSegs[0] == new_nSegs[0] && nSegs[1] == new_nSegs[1]) return false;

#ifdef USE_OPENSG
#ifndef USE_OPENSG_STANDALONE
  //ifxLog(ifxLogLevel::IFX_NORMAL, "Updating arc quad.\n"); 
#endif // USE_OPENSG_STANDALONE
#else
  //std::cout << "Number of segments changed from " << nSegs[0] << " to " << new_nSegs[0] << std::endl;
  //std::cout << "Number of segments changed from " << nSegs[1] << " to " << new_nSegs[1] << std::endl;
#endif

  setNSegs(new_nSegs);
  
  return true;
}

void ArcQuad::draw(bool doUpdateGLBuffer) {

  if (m_disabled) return;

  if(doUpdateGLBuffer) updateGLBuffer();

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef USE_OPENSG

#else
  glEnableVertexAttribArray(0);
    
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

  if(nSegs[0] == nSegs[1])
    glDrawArrays(bufferDrawPrimType, 0, nVertices);
  else
    glDrawArrays(bufferDrawPrimType, 0, nVertices);

  glDisableVertexAttribArray(0);
#endif

}

void ArcQuad::setNSegs(const std::array<int32_t, 2> & _nSegs)
{
  nSegs[0] = _nSegs[0];
  nSegs[1] = _nSegs[1];

  createBuffer();
  createAABB();
}

int ArcRep::ArcQuad::getNumGenTriangles() const
{
  if (m_disabled)
    return 0;

  return 2 * nSegs[0];
}

