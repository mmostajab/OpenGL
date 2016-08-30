#include "ArcQuad.h"

// STD
#include <iostream>

#include "ArcPrimitiveHelper.h"

void ArcQuad::createBuffer() {

      nSegs[0] = _nSegs;
      nSegs[1] = _nSegs;

      if (buffer <= 0 || (2 * (_nSegs + 2)) > bufferSizeMaxNumSegs[0]) {
        
        if (buffer > 0) glDeleteBuffers(1, &buffer);
        
        bufferSizeMaxNumSegs[0] = bufferSizeMaxNumSegs[0] = 2 * (_nSegs + 2);
        
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSizeMaxNumSegs[0] * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
      }

      std::vector<Vertex> vertices;

      Vertex v;

      glm::vec3 a[] = { halfArcQuad[0].p0 - halfArcQuad[0].center, halfArcQuad[1].p0 - halfArcQuad[1].center };
      glm::vec3 b[] = { halfArcQuad[0].p1 - halfArcQuad[0].center, halfArcQuad[1].p1 - halfArcQuad[1].center };
      float cos_alpha = glm::clamp(glm::dot(a[1], b[1]) / (glm::length(a[1]) * glm::length(b[1])), -1.0f, 1.0f);
      //float epsilon() = 0.01f;
      float alpha = acosf(cos_alpha);

      for (int j = 0; j < nSegs[0] + 1; j++) {
        float t = static_cast<float>(j) / static_cast<float>(nSegs[0]);
        float thetha = t * alpha;

        glm::vec3 p[2];

#ifdef USE_SLERP
        p[0] = ArcPrimitiveHelper::interpolation_complex(a[0], b[0], thetha, alpha);
        p[1] = ArcPrimitiveHelper::interpolation_complex(a[1], b[1], thetha, alpha);
#else
        p[0] = ArcPrimitiveHelper::slerp(a[0], b[0], thetha, alpha);
        p[1] = ArcPrimitiveHelper::slerp(a[1], b[1], thetha, alpha);
#endif

        v.position = halfArcQuad[0].center + p[0] ;
        vertices.push_back(v);

        v.position = halfArcQuad[1].center + p[1] ;
        vertices.push_back(v);
      }

      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

      nVertices = static_cast<GLint>(vertices.size());      
  }

void ArcQuad::updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h) {

  int curve_length = 0;

  for (size_t i = 0; i < halfArcQuad.size(); i++) {
    curve_length = ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, halfArcQuad[i].p0, halfArcQuad[i].p1, halfArcQuad[i].center);
  }

  createBuffer(/*static_cast<int>(curve_length / mult)*/);
}

void ArcQuad::draw() {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnableVertexAttribArray(0);
    
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);

  glDisableVertexAttribArray(0);

}