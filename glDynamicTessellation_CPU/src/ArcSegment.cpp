#include "ArcSegment.h"

#include "ArcPrimitiveHelper.h"

// STD
#include <iostream>

ArcSegment::ArcSegment(
  glm::vec3 _p1,
  glm::vec3 _p2,
  glm::vec3 _center,
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

  if (buffer > 0) {
    glDeleteBuffers(1, &buffer);
    buffer = 0;
  }

  std::vector<Vertex> vertices;

  Vertex v;
  glm::vec3 center_of_arc = (p1 + p2) / 2.0f;
  v.position = center_of_arc;
  vertices.push_back(v);

  glm::vec3 a = p1 - center;
  glm::vec3 b = p2 - center;
  float cos_alpha = glm::clamp(glm::dot(a, b) / (glm::length(a) * glm::length(b)), -1.0f, 1.0f);
  float alpha = acosf(cos_alpha);

  for (int i = 0; i < nSegs + 1; i++) {
    float t = static_cast<float>(i) / static_cast<float>(nSegs);
    float thetha = t * alpha;

    glm::vec3 p;

#ifdef USE_SLERP
    p = ArcPrimitiveHelper::slerp(a, b, thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
    p = ArcPrimitiveHelper::interpolation_complex(a, b, thetha, alpha);
#endif

    v.position = p + center;
    vertices.push_back(v);
  }

  glCreateBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

  nVertices = static_cast<GLint>(vertices.size());
}

void ArcSegment::updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h) {

  int new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, p1, p2, center) / mult);

  // if the buffer does not need to change
  if (nSegs == new_nSegs) return;

  std::cout << "Number of segments changed from " << nSegs << " to " << new_nSegs << std::endl;
  setNSegs(new_nSegs);
  createBuffer();

}

void ArcSegment::draw() {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

  glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

  glDisableVertexAttribArray(0);

}

void ArcSegment::setNSegs(const int & _nSegs)
{
  nSegs = _nSegs;
}