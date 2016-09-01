#include "ArcQuad.h"

// STD
#include <iostream>

#include "ArcPrimitiveHelper.h"

ArcQuad::ArcQuad(Vector3D _p1, Vector3D _p2, Vector3D _center0, Vector3D _p3, Vector3D _p4, Vector3D _center1, int _nSegs): 
  DynTessArcPrimitive(DYN_TESS_ARC_QUAD)
{
  halfArcQuad[0].p0 = _p1;
  halfArcQuad[0].p1 = _p2;
  halfArcQuad[0].center = _center0;

  halfArcQuad[1].p0 = _p3;
  halfArcQuad[1].p1 = _p4;
  halfArcQuad[1].center = _center1;

  setNSegs(_nSegs);
  createBuffer();
}

void ArcQuad::createBuffer() {

      if (
#ifdef USE_OPENSG
        0
#else
        buffer <= 0
#endif
        || (2 * (nSegs[0] + 2)) > bufferSizeMaxNumSegs[0]) {
        
        
#ifdef USE_OPENSG
#else
        if (buffer > 0) 
          glDeleteBuffers(1, &buffer);
#endif
        
        bufferSizeMaxNumSegs[0] = bufferSizeMaxNumSegs[1] = 2 * (nSegs[0] + 2);
#ifdef USE_OPENSG  
#else
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSizeMaxNumSegs[0] * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
#endif
      }

      std::vector<Vertex> vertices;

      Vertex v;

      Vector3D a[] = { halfArcQuad[0].p0 - halfArcQuad[0].center, halfArcQuad[1].p0 - halfArcQuad[1].center };
      Vector3D b[] = { halfArcQuad[0].p1 - halfArcQuad[0].center, halfArcQuad[1].p1 - halfArcQuad[1].center };
      float cos_alpha = ArcPrimitiveHelper::angle_between(a[1], b[1]);
      //float epsilon() = 0.01f;
      float alpha = acosf(cos_alpha);

      for (int j = 0; j < nSegs[0] + 1; j++) {
        float t = static_cast<float>(j) / static_cast<float>(nSegs[0]);
        float thetha = t * alpha;

        Vector3D p[2];

#ifdef USE_SLERP
      p[0] = ArcPrimitiveHelper::slerp(a[0], b[0], thetha, alpha);
      p[1] = ArcPrimitiveHelper::slerp(a[1], b[1], thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
        p[0] = ArcPrimitiveHelper::interpolation_complex(a[0], b[0], thetha, alpha);
        p[1] = ArcPrimitiveHelper::interpolation_complex(a[1], b[1], thetha, alpha);
#endif

        v.position = halfArcQuad[0].center + p[0] ;
        vertices.push_back(v);

        v.position = halfArcQuad[1].center + p[1] ;
        vertices.push_back(v);
      }

#ifdef USE_OPENSG

#else
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
#endif
      nVertices = static_cast<GLint>(vertices.size());      
  }

void ArcQuad::updateBuffer(Matrix4x4 mvp, unsigned int w, unsigned int h) {

  int new_nSegs = 0;

  for (size_t i = 0; i < halfArcQuad.size(); i++) {
    new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, halfArcQuad[i].p0, halfArcQuad[i].p1, halfArcQuad[i].center) / m_tessScale);
  }

  // if the buffer does not need to change
  if (nSegs[0] == new_nSegs) return;

  std::cout << "Number of segments changed from " << nSegs[0] << " to " << new_nSegs << std::endl;
  setNSegs(new_nSegs);
  createBuffer();
}

void ArcQuad::draw() {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef USE_OPENSG

#else
  glEnableVertexAttribArray(0);
    
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);

  glDisableVertexAttribArray(0);
#endif

}

void ArcQuad::setNSegs(const int & _nSegs)
{
  nSegs[0] = nSegs[1] = _nSegs;
}