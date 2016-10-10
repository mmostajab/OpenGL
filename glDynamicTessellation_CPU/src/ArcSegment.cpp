#include "ArcSegment.h"
#include "ArcPrimitiveHelper.h"

// STD
#include <iostream>

using namespace ArcRep;

ArcSegment::ArcSegment(
  Vector3Df _p1,
  Vector3Df _p2,
  Vector3Df _center,
  int _nSegs): DynTessArcPrimitive(DYN_TESS_ARC_SEGMENT) {

  set(_p1, _p2, _center, _nSegs);

}

void ArcSegment::set(
  Vector3Df _p1,
  Vector3Df _p2,
  Vector3Df _center,
  int _nSegs) {

  p1 = _p1;
  p2 = _p2;
  center = _center;

  setNSegs(_nSegs);
  createBuffer();

}

void ArcSegment::createBuffer() {

#ifdef USE_OPENSG
  vertices.clear();
  lengths.clear();
  types.clear();
#else

  if (buffer > 0) {
    glDeleteBuffers(1, &buffer);
    buffer = 0;
  }

  std::vector<Vertex> vertices;
#endif

  Vertex v;
  Vector3Df center_of_arc = (p1 + p2) * 0.5f;
  v.position = center_of_arc;

#ifdef USE_OPENSG
  vertices.push_back(OSG::Pnt3f(v.position));
#else
  vertices.push_back(v);
#endif

  Vector3Df a = p1 - center;
  Vector3Df b = p2 - center;
  float cos_alpha = ArcPrimitiveHelper::angle_between(a, b);
  float alpha = acosf(cos_alpha);

  for (int i = 0; i < nSegs + 1; i++) {
    float t = static_cast<float>(i) / static_cast<float>(nSegs);
    float thetha = t * alpha;

    Vector3Df p;

#ifdef USE_SLERP
    p = ArcPrimitiveHelper::slerp(a, b, thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
    p = ArcPrimitiveHelper::interpolation_complex(a, b, thetha, alpha);
#endif

    v.position = p + center;
#ifdef USE_OPENSG
    vertices.push_back(OSG::Pnt3f(v.position));
#else
    vertices.push_back(v);
#endif
  }

#ifdef USE_OPENSG

  lengths.push_back(static_cast<OSG::UInt32>(vertices.size()));
	types.push_back(GL_TRIANGLE_FAN);

  //createDrawArraysNode(transform, vertices, lengths, types);  
#else
  glCreateBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
#endif

  nVertices = static_cast<GLint>(vertices.size());
}

bool ArcSegment::updateBuffer(Matrix4x4f mvp, unsigned int w, unsigned int h) {

  int new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, p1, p2, center) / m_tessScale);

  // if the buffer does not need to change
  if (nSegs == new_nSegs) return false;

#ifdef USE_OPENSG
  ifxLog( ifxLogLevel::IFX_NORMAL, "Updating arc segment.\n"); 
#else
  //std::cout << "Number of segments changed from " << nSegs << " to " << new_nSegs << std::endl;
#endif

  setNSegs(new_nSegs);
  createBuffer();
  return true;
}

void ArcSegment::draw() {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef USE_OPENSG
  // Draw will be called in the node.
#else
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

  glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

  glDisableVertexAttribArray(0);
#endif
}

void ArcSegment::setNSegs(const int & _nSegs)
{
  nSegs = _nSegs;
}