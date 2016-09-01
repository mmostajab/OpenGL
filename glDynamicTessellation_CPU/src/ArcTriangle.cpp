#include "ArcTriangle.h"

#include "ArcPrimitiveHelper.h"

// STD
#include <iostream>

ArcTriangle::ArcTriangle(
    Vector3D _p1,
    Vector3D _p2,
    Vector3D _p3,
    Vector3D _center/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs): DynTessArcPrimitive(DYN_TESS_ARC_TRIANGLE) {

    p1 = _p1;
    p2 = _p2;
    p3 = _p3;
    center = _center;
    //alpha = _alpha;
    setNSegs(_nSegs);
    createBuffer();
  }

  void ArcTriangle::createBuffer() {

    if (buffer > 0) {
#ifdef USE_OPENSG

#else
      glDeleteBuffers(1, &buffer);
      buffer = 0;
#endif
    }

    std::vector<Vertex> vertices;

    Vertex v;
    v.position = p3;
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
      p = ArcPrimitiveHelper::interpolation_complex(a, b, -thetha, -alpha);
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

  void ArcTriangle::updateBuffer(Matrix4x4 mvp, unsigned int w, unsigned int h) {
    
    int new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, p1, p2, center) / m_tessScale);

    if (new_nSegs == nSegs) return;

    std::cout << "Number of segments changed from " << nSegs << " to " << new_nSegs << std::endl;
    setNSegs(new_nSegs);
    createBuffer();

  }

  void ArcTriangle::draw() {

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

void ArcTriangle::setNSegs(const int& _nSegs)
{
  nSegs = _nSegs;
}
