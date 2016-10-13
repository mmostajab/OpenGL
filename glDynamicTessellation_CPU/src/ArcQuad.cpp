#include "ArcQuad.h"

// STD
#include <iostream>

#include "ArcPrimitiveHelper.h"

using namespace ArcRep;

ArcQuad::ArcQuad(Vector3Df _p1, Vector3Df _p2, Vector3Df _center0, Vector3Df _p3, Vector3Df _p4, Vector3Df _center1, int _nSegs): 
  DynTessArcPrimitive(DYN_TESS_ARC_QUAD)
{
  set(_p1, _p2, _center0, _p3, _p4, _center1, _nSegs);
}

void ArcQuad::set(Vector3Df _p1, Vector3Df _p2, Vector3Df _center0, Vector3Df _p3, Vector3Df _p4, Vector3Df _center1, int _nSegs)
{
  halfArcQuad[0].p0 = _p1;
  halfArcQuad[0].p1 = _p2;
  halfArcQuad[0].center = _center0;

  halfArcQuad[1].p0 = _p3;
  halfArcQuad[1].p1 = _p4;
  halfArcQuad[1].center = _center1;

  std::array<int32_t, 2> NumberOfSegments = { _nSegs, _nSegs };
  setNSegs(NumberOfSegments);
  createBuffer();
}

void ArcQuad::createBuffer() {

#ifdef USE_OPENSG
  vertices.clear();
  lengths.clear();
  types.clear();
#else
  std::vector<Vertex> vertices;
#endif

  std::vector<Vector3Df> curve_points[2];
  for (int i = 0; i < 2; i++){
    Vector3Df a     = halfArcQuad[i].p0 - halfArcQuad[i].center;
    Vector3Df b     = halfArcQuad[i].p1 - halfArcQuad[i].center;
    float alpha = ArcPrimitiveHelper::angle_between(a, b);

    for (int j = 0; j < nSegs[0] + 1; j++) {
      float t = static_cast<float>(j) / static_cast<float>(nSegs[0]);
      float thetha = t * alpha;

      Vector3Df p;

#ifdef USE_SLERP
      p = ArcPrimitiveHelper::slerp(a, b, thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
      p = ArcPrimitiveHelper::interpolation_complex(a, b, thetha, alpha);
#endif
      curve_points[i].push_back(p + halfArcQuad[i].center);
    };
  }
    

  Vertex v;

  if(curve_points[0].size() == curve_points[1].size()){

    for(size_t i = 0; i < curve_points[0].size(); i++){

#ifdef USE_OPENSG
      v.position = curve_points[0][i];
      vertices.push_back(OSG::Pnt3f(v.position));

      v.position = curve_points[1][i];
      vertices.push_back(OSG::Pnt3f(v.position));
#else
      v.position = curve_points[0][i];
      vertices.push_back(v);
      
      v.position = curve_points[1][i];
      vertices.push_back(v);
#endif

    }

#ifdef USE_OPENSG
    lengths.push_back(static_cast<OSG::UInt32>(vertices.size()));
    types.push_back(GL_TRIANGLE_STRIP);
#endif

  } else {

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

  }

#ifdef USE_OPENSG
  //createDrawArraysNode(transform, vertices, lengths, types);  
#else

  if (buffer > 0)
  glDeleteBuffers(1, &buffer);
  glCreateBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
#endif
      nVertices = static_cast<GLint>(vertices.size());      
  }

bool ArcQuad::updateBuffer(const CameraInfo& camInfo, Matrix4x4f mvp, unsigned int w, unsigned int h) {

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
      }
      else {
        // calculate the new number of segments
        float alpha = acos(tri_alpha);
        float angle = ArcPrimitiveHelper::angle_between(halfArcQuad[i].p0 - halfArcQuad[i].center, halfArcQuad[i].p1 - halfArcQuad[i].center);

        new_nSegs[i] = static_cast<int>(std::ceil(angle / (m_tessScale * m_triangulationAccuracy * alpha)));
      }
    }

  }

  new_nSegs[0] = new_nSegs[1] = std::max(new_nSegs[0], new_nSegs[1]);

  // if the buffer does not need to change
#ifdef UPDATE_ARCS_EVERY_FRAME
  if (nSegs[0] == new_nSegs[0] && nSegs[1] == new_nSegs[1]) return false;
#endif

#ifdef USE_OPENSG
  ifxLog( ifxLogLevel::IFX_NORMAL, "Updating arc quad.\n"); 
#else
  //std::cout << "Number of segments changed from " << nSegs[0] << " to " << new_nSegs[0] << std::endl;
  //std::cout << "Number of segments changed from " << nSegs[1] << " to " << new_nSegs[1] << std::endl;
#endif

  setNSegs(new_nSegs);
  createBuffer();

  return true;
}

void ArcQuad::draw() {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef USE_OPENSG

#else
  glEnableVertexAttribArray(0);
    
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

  if(nSegs[0] == nSegs[1])
    glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);
  else
    glDrawArrays(GL_TRIANGLES, 0, nVertices);

  glDisableVertexAttribArray(0);
#endif

}

void ArcQuad::setNSegs(const std::array<int32_t, 2> & _nSegs)
{
  nSegs[0] = _nSegs[0];
  nSegs[1] = _nSegs[1];
}

int ArcRep::ArcQuad::getNumGenTriangles() const
{
  return 2 * nSegs[0];
}
