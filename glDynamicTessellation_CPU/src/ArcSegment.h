#ifndef __ARC_SEGMENT_H__
#define __ARC_SEGMENT_H__

#include <glm\glm.hpp>
#include <GL\GL.h>
#include <vector>

extern float mult;

struct ArcSegment
{									//      ___       ;
  glm::vec3 	p1;                 //    /     \     ;
  glm::vec3 	p2;                 // p1 ------- p2  ;
                                  //double	    alpha; // arc angle in rad
  glm::vec3   center;

  struct Vertex {
    glm::vec3 position;
  };

  GLuint buffer = 0;
  GLint  nVertices;
  int    nSegs = -1;

  ArcSegment(
    glm::vec3 _p1 = glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3 _p2 = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 _center = glm::vec3(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs = 5) {

    p1 = _p1;
    p2 = _p2;
    center = _center;
    //alpha = _alpha;
    createBuffer(_nSegs);
  }

  void createBuffer(int _nSegs) {

    if (_nSegs == nSegs) return;

    std::cout << "Number of segments changed from " << nSegs << " to " << _nSegs << std::endl;

    nSegs = _nSegs;

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
      glm::vec3 p =
        sinf(alpha - thetha) / sinf(static_cast<float>(alpha)) * a +
        sinf(thetha) / sinf(static_cast<float>(alpha)) * b;

      //std::complex<float> numerator = (1.f - std::exp(std::complex<float>(0.0f, -thetha)));
      //std::complex<float> divisor   = (1.f - std::exp(std::complex<float>(0.0f, -alpha)));
      //std::complex<float> w = numerator / divisor;
      //v.position = w.real() * p2 + (1 - w.real()) * p1;

      v.position = p + center;
      vertices.push_back(v);
    }

    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    nVertices = static_cast<GLint>(vertices.size());
  }

  void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h) {

    glm::vec4 p1_proj = mvp * glm::vec4(p1, 1.0f);
    glm::vec4 p2_proj = mvp * glm::vec4(p2, 1.0f);
    glm::vec4 center_proj = mvp * glm::vec4(center, 1.0f);

    p1_proj.x /= p1_proj.z;	p1_proj.y /= p1_proj.z;
    p2_proj.x /= p2_proj.z;	p2_proj.y /= p2_proj.z;
    center_proj.x /= center_proj.z; center_proj.y /= center_proj.z;

    //glm::vec2 len = glm::vec2((p1_proj.x - p2_proj.x) * w, (p1_proj.y - p2_proj.y) * h);
    //std::cout << "Length = " << glm::length(len) << std::endl;

    float radius = glm::max(glm::length(p1_proj - center_proj) * w / 2.0f, glm::length(p2_proj - center_proj) * h / 2.0f); //glm::length(len) / (2 * sin(static_cast<float>(alpha)));
    float alpha =
      acosf(glm::dot(p1_proj - center_proj, p2_proj - center_proj)
        / (glm::length(p1_proj - center_proj) * glm::length(p1_proj - center_proj)));

    int curve_length = static_cast<int>(radius * static_cast<float>(alpha)) + 1;
    createBuffer(static_cast<int>(curve_length / mult));

  }

  void drawBuffer() {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

    glDisableVertexAttribArray(0);

  }
};

#endif