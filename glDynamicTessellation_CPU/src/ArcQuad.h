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

  std::array<GLuint,  2> buffer     = {  0,  0 };
  std::array<size_t, 2>  bufferSizeMaxNumSegs = {  0,  0 };
  std::array<int32_t, 2> nVertices  = { -1, -1 };
  std::array<int32_t, 2> nSegs      = { -1, -1 };

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
    for (size_t i = 0; i < halfArcQuad.size(); i++) createBuffer(i, _nSegs);
  }

  void createBuffer(size_t i, uint32_t _nSegs) {

    //if (_nSegs0 == nSegs[0] && _nSegs1 == nSegs[1]) return;

    //for (size_t i = 0; i < buffer.size(); i++) {

      if (_nSegs == nSegs[i]) return;

      std::cout << "Number of segments for curve " << i << " changed from " << nSegs[i] << " to " << _nSegs << "." << std::endl;

      nSegs[i] = _nSegs;

      if (buffer[i] > 0 || _nSegs > bufferSizeMaxNumSegs[i]) {
        
        if(buffer[i] > 0) glDeleteBuffers(1, &buffer[i]);
        
        //buffer[i] = 0;
        bufferSizeMaxNumSegs[i] = _nSegs + 2;
        glCreateBuffers(1, &buffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
        glBufferData(GL_ARRAY_BUFFER, bufferSizeMaxNumSegs[i] * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
      }

      std::vector<Vertex> vertices;

      Vertex v;
      v.position = i == 0 ? halfArcQuad[1].p1 : halfArcQuad[0].p0;// halfArcQuad[i].center;
      vertices.push_back(v);

      glm::vec3 a = halfArcQuad[i].p0 - halfArcQuad[i].center;
      glm::vec3 b = halfArcQuad[i].p1 - halfArcQuad[i].center;
      float cos_alpha = glm::clamp(glm::dot(a, b) / (glm::length(a) * glm::length(b)), -1.0f, 1.0f);
      float alpha = acosf(cos_alpha);

      for (int j = 0; j < nSegs[i] + 1; j++) {
        float t = static_cast<float>(j) / static_cast<float>(nSegs[i]);
        float thetha = t * alpha;
        glm::vec3 p =
          sinf(alpha - thetha) / sinf(static_cast<float>(alpha)) * a +
          sinf(thetha) / sinf(static_cast<float>(alpha)) * b;

        v.position = p + halfArcQuad[i].center;
        vertices.push_back(v);
      }

      glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

      nVertices[i] = static_cast<GLint>(vertices.size());      
    //}    
  }

  void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h) {

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
        acosf(glm::dot(p1_proj - center_proj, p2_proj - center_proj)
          / (glm::length(p1_proj - center_proj) * glm::length(p1_proj - center_proj)));

      int curve_length = static_cast<int>(radius * static_cast<float>(alpha)) + 1;
      createBuffer(i, static_cast<int>(curve_length / mult));
    }
  }

  void drawBuffer() {

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnableVertexAttribArray(0);
    
    for (size_t i = 0; i < buffer.size(); i++) {
      glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);
      glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices[i]);
    }

    glDisableVertexAttribArray(0);

  }
};

#endif // __ARC_QUAD_H__