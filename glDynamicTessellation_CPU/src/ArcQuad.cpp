#ifndef __ARC_QUAD_H__
#define __ARC_QUAD_H__

#include <glm\glm.hpp>
#include <GL\GL.h>

#include <array>
#include <vector>

extern float mult;

struct ArcQuad
{
  struct HalfArcQuad {
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec3 center;
  };

  struct Vertex {
    glm::vec3 position;
  };

  std::array<HalfArcQuad, 2> halfArcQuad;

  GLuint                  buffer               = 0;
  std::array<size_t,  2>  bufferSizeMaxNumSegs = {  0,  0 };
  int32_t                 nVertices            = -1;
  std::array<int32_t, 2>  nSegs                = { -1, -1 };

  ArcQuad(
    glm::vec3 _p1      = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 _p2      = glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3 _center0 = glm::vec3(0.5f, 0.0f, 0.0f)/*double _alpha0 = glm::pi<double>() / 2.0f*/,
    glm::vec3 _p3      = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 _p4      = glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3 _center1 = glm::vec3(0.5f, 1.0f, 0.0f)/*double _alpha1 = glm::pi<double>() / 2.0f*/,
    int _nSegs         = 5) {

    halfArcQuad[0].p0     = _p1;
    halfArcQuad[0].p1     = _p2;
    halfArcQuad[0].center = _center0;

    halfArcQuad[1].p0     = _p3;
    halfArcQuad[1].p1     = _p4;
    halfArcQuad[1].center = _center1;

    createBuffer(_nSegs);
  }

  void createBuffer(uint32_t _nSegs) {

    //if (_nSegs0 == nSegs[0] && _nSegs1 == nSegs[1]) return;

    //for (size_t i = 0; i < buffer.size(); i++) {

      if (_nSegs == nSegs[0] && _nSegs == nSegs[1]) return;

      //std::cout << "Number of segments for curve " << i << " changed from " << nSegs[i] << " to " << _nSegs << "." << std::endl;

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

//#ifdef USE_SLERP
//        glm::vec3 p[] = {
//          // 0
//          sinf(alpha - thetha) / sinf(static_cast<float>(alpha)) * a[0] +
//          sinf(thetha) / sinf(static_cast<float>(alpha)) * b[0],
//
//          // 1
//          sinf(alpha - thetha) / sinf(static_cast<float>(alpha)) * a[1] +
//          sinf(thetha) / sinf(static_cast<float>(alpha)) * b[1]
//        };
//#endif

//#ifdef USE_COMPLEX_METHOD
        std::complex<float> numerator = (1.f - (std::complex<float>(cos(thetha), sin(thetha))));
        std::complex<float> divisor = (1.f - std::complex<float>(cos(alpha), sin(alpha)));
        std::complex<float> w = numerator / divisor;

        std::complex<float> p_complex[] = {
          (1.f - w) * std::complex<float>(b[0].x, b[0].y) + w * std::complex<float>(a[0].x, a[0].y),
          (1.f - w) * std::complex<float>(b[1].x, b[1].y) + w * std::complex<float>(a[1].x, a[1].y)
        };
        glm::vec3 p[] = {
          glm::vec3(p_complex[0].real(), p_complex[0].imag(), 0.0f),
          glm::vec3(p_complex[1].real(), p_complex[1].imag(), 0.0f)
        };
//#endif

        v.position = halfArcQuad[0].center + p[0] ;
        vertices.push_back(v);

        v.position = halfArcQuad[1].center + p[1] ;
        vertices.push_back(v);
      }

      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

      nVertices = static_cast<GLint>(vertices.size());      
    //}    
  }

  void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h) {

    int curve_length = 0;

    for (size_t i = 0; i < halfArcQuad.size(); i++) {

      glm::vec4 p1_proj     = mvp * glm::vec4(halfArcQuad[i].p0,      1.0f);
      glm::vec4 p2_proj     = mvp * glm::vec4(halfArcQuad[i].p1,      1.0f);
      glm::vec4 center_proj = mvp * glm::vec4(halfArcQuad[i].center,  1.0f);

      p1_proj.x     /= p1_proj.z;	    p1_proj.y     /= p1_proj.z;
      p2_proj.x     /= p2_proj.z;	    p2_proj.y     /= p2_proj.z;
      center_proj.x /= center_proj.z; center_proj.y /= center_proj.z;

      //glm::vec2 len = glm::vec2((p1_proj.x - p2_proj.x) * w, (p1_proj.y - p2_proj.y) * h);
      //std::cout << "Length = " << glm::length(len) << std::endl;

      float radius = glm::max(glm::length(p1_proj - center_proj) * w / 2.0f, glm::length(p2_proj - center_proj) * h / 2.0f); //glm::length(len) / (2 * sin(static_cast<float>(alpha)));
      float alpha =
        acosf(
          glm::clamp(
          glm::dot(p1_proj - center_proj, p2_proj - center_proj)
          / (glm::length(p1_proj - center_proj) * glm::length(p2_proj - center_proj)), -1.0f, 1.0f
      ));

       curve_length = glm::max(static_cast<int>(radius * static_cast<float>(alpha)) + 1, curve_length);
      
    }

    createBuffer(static_cast<int>(curve_length / mult));
  }

  void drawBuffer() {

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);

    glDisableVertexAttribArray(0);

  }
};

#endif // __ARC_QUAD_H__