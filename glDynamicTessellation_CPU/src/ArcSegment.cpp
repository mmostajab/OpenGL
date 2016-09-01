#include "ArcSegment.h"

#include "ArcPrimitiveHelper.h"

// STD
#include <iostream>

ArcSegment::ArcSegment(
  Vector3D _p1,
  Vector3D _p2,
  Vector3D _center,
  int _nSegs): DynTessArcPrimitive(DYN_TESS_ARC_SEGMENT) {

  p1 = _p1;
  p2 = _p2;
  center = _center;
  //alpha = _alpha;
  //_nSegs = nSegs;
  setNSegs(_nSegs);
  createBuffer();
}

void ArcSegment::createBuffer() {

#ifdef USE_OPENSG
  
#else
  if (buffer > 0) {
    glDeleteBuffers(1, &buffer);
    buffer = 0;
  }
#endif

  std::vector<Vertex> vertices;

  Vertex v;
  Vector3D center_of_arc = (p1 + p2) * 0.5f;
  v.position = center_of_arc;
  vertices.push_back(v);

  Vector3D a = p1 - center;
  Vector3D b = p2 - center;
  float cos_alpha = ArcPrimitiveHelper::angle_between(a, b);
  float alpha = acosf(cos_alpha);

  for (int i = 0; i < nSegs + 1; i++) {
    float t = static_cast<float>(i) / static_cast<float>(nSegs);
    float thetha = t * alpha;

    Vector3D p;

#ifdef USE_SLERP
    p = ArcPrimitiveHelper::slerp(a, b, thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
    p = ArcPrimitiveHelper::interpolation_complex(a, b, thetha, alpha);
#endif

    v.position = p + center;
    vertices.push_back(v);
  }

  #ifdef USE_OPENSG
  
#else
  glCreateBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
#endif

  nVertices = static_cast<GLint>(vertices.size());
}

void ArcSegment::updateBuffer(Matrix4x4 mvp, unsigned int w, unsigned int h) {

  int new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, p1, p2, center) / m_tessScale);

  // if the buffer does not need to change
  if (nSegs == new_nSegs) return;

  std::cout << "Number of segments changed from " << nSegs << " to " << new_nSegs << std::endl;
  setNSegs(new_nSegs);
  createBuffer();

}

void ArcSegment::draw() {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef USE_OPENSG
  
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