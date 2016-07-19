#ifndef __GRAPHICS_SHAPES_H__
#define __GRAPHICS_SHAPES_H__

// STD
#include <vector>
#include <stdint.h>

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>

// ===================================================================
// ===================================================================
// == Directed Box
// ===================================================================
// ===================================================================

namespace Graphics {
  struct DirectedBox {

    struct Box {
      glm::vec3     start;
      glm::vec3     end;
      glm::float32  width;
      glm::float32  height;
      glm::int32    sign;
      glm::int32    padding[2];
    };

    GLuint           gl_shader_program;
    GLuint           gl_directedBoxes_buffer;

    std::vector<DirectedBox::Box> directedBoxes_buffer;

    DirectedBox() {
      gl_shader_program       = 0;
      gl_directedBoxes_buffer = 0;
    }

    ~DirectedBox() {
      if (gl_shader_program > 0)          glDeleteShader(gl_shader_program);
      if (gl_directedBoxes_buffer  > 0)   glDeleteBuffers(1, &gl_directedBoxes_buffer);

      gl_shader_program       = 0;
      gl_directedBoxes_buffer = 0;
    }

    void addDirectedBox(const glm::vec3& start, const glm::vec3& end, const glm::float32& width, const glm::float32& height, int sign = 1) {
      DirectedBox::Box box;
      const float scale = 0.0002f;
      box.start   = scale * start;
      box.end     = scale * end;
      box.width   = scale * width;
      box.height  = scale * height;
      box.sign    = sign;
      directedBoxes_buffer.push_back(box);
    }

    /// recreates the opengl buffers
    void createGLBuffer() {

      if (gl_directedBoxes_buffer > 0)  glDeleteBuffers(1, &gl_directedBoxes_buffer);

      glGenBuffers(1, &gl_directedBoxes_buffer);


      glBindBuffer(GL_ARRAY_BUFFER, gl_directedBoxes_buffer);
      glBufferData(GL_ARRAY_BUFFER, directedBoxes_buffer.size() * sizeof(DirectedBox::Box), directedBoxes_buffer.data(), GL_STATIC_DRAW);
    }

    /// draws the buffers
    void draw(glm::mat4 mvp) {
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, gl_directedBoxes_buffer);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DirectedBox::Box), (char*)0 + offsetof(DirectedBox::Box, start));

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, gl_directedBoxes_buffer);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DirectedBox::Box), (char*)0 + offsetof(DirectedBox::Box, end));

      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, gl_directedBoxes_buffer);
      glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(DirectedBox::Box), (char*)0 + offsetof(DirectedBox::Box, width));

      glEnableVertexAttribArray(3);
      glBindBuffer(GL_ARRAY_BUFFER, gl_directedBoxes_buffer);
      glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(DirectedBox::Box), (char*)0 + offsetof(DirectedBox::Box, height));

      glEnableVertexAttribArray(4);
      glBindBuffer(GL_ARRAY_BUFFER, gl_directedBoxes_buffer);
      glVertexAttribPointer(4, 1, GL_INT,   GL_FALSE, sizeof(DirectedBox::Box), (char*)0 + offsetof(DirectedBox::Box, sign));

      glUseProgram(gl_shader_program);
      glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&mvp);

      glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(directedBoxes_buffer.size()));

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
      glDisableVertexAttribArray(3);
      glDisableVertexAttribArray(4);
    }

  };
}

// ===================================================================
// ===================================================================
// == Box
// ===================================================================
// ===================================================================

namespace Graphics {
  struct Boxes {

    GLuint           gl_shader_program;
    GLuint           gl_centers_buffer;
    GLuint           gl_signs_buffer;
    GLuint           gl_dims_buffer;

    std::vector<glm::vec3>  centers_buffer;
    std::vector<glm::int32> sign_buffer;
    std::vector<glm::vec3>  dims_buffer;

    /// Constructor
    Boxes() {
      gl_shader_program = 0;
      gl_centers_buffer = 0;
      gl_dims_buffer    = 0;
      gl_signs_buffer   = 0;

      centers_buffer.clear();
      dims_buffer.clear();
    }

    /// destructor
    ~Boxes() {
      if (gl_shader_program > 0)  glDeleteShader(gl_shader_program);
      if (gl_centers_buffer > 0)  glDeleteBuffers(1, &gl_centers_buffer);
      if (gl_dims_buffer > 0)     glDeleteBuffers(1, &gl_dims_buffer);
      if (gl_signs_buffer > 0)    glDeleteBuffers(1, &gl_signs_buffer);

      gl_shader_program = 0;
      gl_centers_buffer = 0;
      gl_dims_buffer    = 0;
      gl_signs_buffer   = 0;
    }

    /// adds a box to the vector
    void addBox(glm::vec3 center, glm::vec3 dims, int sign = 1) {
      const float scale = 0.0002f;
      centers_buffer.push_back(scale * center);
      dims_buffer.push_back(scale * dims);
      sign_buffer.push_back(sign);
    }

    /// recreates the opengl buffers
    void createGLBuffer() {

      if (gl_centers_buffer > 0)  glDeleteBuffers(1, &gl_centers_buffer);
      if (gl_dims_buffer > 0)     glDeleteBuffers(1, &gl_dims_buffer);
      if (gl_signs_buffer > 0)    glDeleteBuffers(1, &gl_signs_buffer);

      glGenBuffers(1, &gl_centers_buffer);
      glGenBuffers(1, &gl_dims_buffer);
      glGenBuffers(1, &gl_signs_buffer);

      glBindBuffer(GL_ARRAY_BUFFER, gl_centers_buffer);
      glBufferData(GL_ARRAY_BUFFER, centers_buffer.size() * sizeof(glm::vec3), centers_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_dims_buffer);
      glBufferData(GL_ARRAY_BUFFER, dims_buffer.size() * sizeof(glm::float32), dims_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_signs_buffer);
      glBufferData(GL_ARRAY_BUFFER, sign_buffer.size() * sizeof(glm::int32), sign_buffer.data(), GL_STATIC_DRAW);

    }

    /// draws the buffers
    void draw(glm::mat4 mvp) {
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, gl_centers_buffer);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, nullptr);

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, gl_dims_buffer);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NULL, nullptr);

      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, gl_signs_buffer);
      glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, NULL, nullptr);

      glUseProgram(gl_shader_program);
      glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&mvp);

      glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(centers_buffer.size()));

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
    }

  };
}

// ===================================================================
// ===================================================================
// == Cylinder
// ===================================================================
// ===================================================================

namespace Graphics {
  struct Cylinders {

    std::vector<glm::vec3>    centers_buffer;
    std::vector<glm::float32> radiuses_buffer;
    std::vector<glm::float32> heights_buffer;
    std::vector<glm::int32>   signs_buffer;

    GLuint           gl_shader_program;
    GLuint           gl_centers_buffer;
    GLuint           gl_radiuses_buffer;
    GLuint           gl_heights_buffer;
    GLuint           gl_signs_buffer;

    /// constructor
    Cylinders() {
      gl_shader_program   = 0;
      gl_centers_buffer   = 0;
      gl_radiuses_buffer  = 0;
      gl_heights_buffer   = 0;
      gl_signs_buffer     = 0;

      centers_buffer.clear();
      radiuses_buffer.clear();
      heights_buffer.clear();
    }

    /// destructor
    ~Cylinders() {
      if (gl_shader_program  > 0) glDeleteShader(gl_shader_program);
      if (gl_centers_buffer  > 0) glDeleteBuffers(1, &gl_centers_buffer);
      if (gl_radiuses_buffer > 0) glDeleteBuffers(1, &gl_radiuses_buffer);
      if (gl_heights_buffer  > 0) glDeleteBuffers(1, &gl_heights_buffer);
      if (gl_signs_buffer    > 0) glDeleteBuffers(1, &gl_signs_buffer);

      gl_shader_program   = 0;
      gl_centers_buffer   = 0;
      gl_radiuses_buffer  = 0;
      gl_heights_buffer   = 0;
      gl_signs_buffer     = 0;
    }

    /// adds a cylinder to the CPU side buffer
    void addCylinder(const glm::vec3& center, const glm::float32& radius, const glm::float32& height, int sign = 1) {
      const float scale = 0.0002f;
      centers_buffer.push_back(center * scale);
      radiuses_buffer.push_back(radius * scale);
      heights_buffer.push_back(height * scale);
      signs_buffer.push_back(sign);
    }

    /// recreates the opengl buffers
    void createGLBuffer() {

      if (gl_centers_buffer  > 0) glDeleteBuffers(1, &gl_centers_buffer);
      if (gl_radiuses_buffer > 0) glDeleteBuffers(1, &gl_radiuses_buffer);
      if (gl_heights_buffer  > 0) glDeleteBuffers(1, &gl_heights_buffer);
      if (gl_signs_buffer    > 0) glDeleteBuffers(1, &gl_signs_buffer);

      glGenBuffers(1, &gl_centers_buffer);
      glGenBuffers(1, &gl_radiuses_buffer);
      glGenBuffers(1, &gl_heights_buffer);
      glGenBuffers(1, &gl_signs_buffer);

      glBindBuffer(GL_ARRAY_BUFFER, gl_centers_buffer);
      glBufferData(GL_ARRAY_BUFFER, centers_buffer.size() * sizeof(glm::vec3), centers_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_radiuses_buffer);
      glBufferData(GL_ARRAY_BUFFER, radiuses_buffer.size() * sizeof(glm::float32), radiuses_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_heights_buffer);
      glBufferData(GL_ARRAY_BUFFER, heights_buffer.size() * sizeof(glm::float32), heights_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_signs_buffer);
      glBufferData(GL_ARRAY_BUFFER, signs_buffer.size() * sizeof(glm::int32), signs_buffer.data(), GL_STATIC_DRAW);
    }

    /// create cpu side and gpu buffers from array of centers, radiuses and heights.
    void create(const std::vector<glm::vec3>& centers, const std::vector<glm::float32>& radiuses, const std::vector<glm::float32>& heights, const std::vector<glm::int32>& signs) {
      centers_buffer = centers;
      radiuses_buffer = radiuses;
      heights_buffer = heights;
      signs_buffer = signs;

      if (gl_centers_buffer  > 0) glDeleteBuffers(1, &gl_centers_buffer);
      if (gl_radiuses_buffer > 0) glDeleteBuffers(1, &gl_radiuses_buffer);
      if (gl_heights_buffer  > 0) glDeleteBuffers(1, &gl_heights_buffer);
      if (gl_signs_buffer    > 0) glDeleteBuffers(1, &gl_signs_buffer);

      glGenBuffers(1, &gl_centers_buffer);
      glGenBuffers(1, &gl_radiuses_buffer);
      glGenBuffers(1, &gl_heights_buffer);
      glGenBuffers(1, &gl_signs_buffer);

      glBindBuffer(GL_ARRAY_BUFFER, gl_centers_buffer);
      glBufferData(GL_ARRAY_BUFFER, centers_buffer.size() * sizeof(glm::vec3),      centers_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_radiuses_buffer);
      glBufferData(GL_ARRAY_BUFFER, radiuses_buffer.size() * sizeof(glm::float32),  radiuses_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_heights_buffer);
      glBufferData(GL_ARRAY_BUFFER, heights_buffer.size() * sizeof(glm::float32),   heights_buffer.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, gl_signs_buffer);
      glBufferData(GL_ARRAY_BUFFER, signs_buffer.size() * sizeof(glm::int32), signs_buffer.data(), GL_STATIC_DRAW);

    }

    /// draws the buffers
    void draw(glm::vec3 eye, glm::mat4 mvp) {
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, gl_centers_buffer);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, nullptr);

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, gl_radiuses_buffer);
      glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, NULL, nullptr);

      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, gl_heights_buffer);
      glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, NULL, nullptr);

      glEnableVertexAttribArray(3);
      glBindBuffer(GL_ARRAY_BUFFER, gl_signs_buffer);
      glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, NULL, nullptr);

      glUseProgram( gl_shader_program );
      glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&mvp);
      glUniform3fv(1, 1, (GLfloat*)&eye);

      glDrawArrays( GL_POINTS, 0, static_cast<GLsizei>( centers_buffer.size() ) );

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
      glDisableVertexAttribArray(3);
    }
  };
}

// ===================================================================
// ===================================================================
// == Polygons
// ===================================================================
// ===================================================================
namespace Graphics {
  struct Polygons {

    typedef  struct {
      uint32_t  count;
      uint32_t  instanceCount;
      uint32_t  first;
      uint32_t  baseInstance;
    } DrawArraysIndirectCommand;

    std::vector<glm::vec3>                  points;
    std::vector<glm::int32>                 signs;
    std::vector<DrawArraysIndirectCommand>  indirectDrawCommands;

    GLuint gl_shader_program;
    GLuint gl_points, gl_signs;
    GLuint gl_indirect_draw_commands;
    
    void setPoints(const std::vector<glm::vec3>& _points) {
      points = _points;
      scalePoints();
      signs.resize(points.size(), 0);
    }

    void addDrawElementsIndirectCommand(GLint firstIndex, GLint count) {
      DrawArraysIndirectCommand indirectCommand;

      if (count < 0) return;

      int sign = count >= 0 ? +1 : -1;
      for (size_t i = 0; i < sign * count; i++) {
        signs[firstIndex + i] = sign;
      }

      indirectCommand.first         = firstIndex;
      indirectCommand.count         = abs(count);
      indirectCommand.instanceCount = 1;
      indirectCommand.baseInstance  = 0;

      indirectDrawCommands.push_back(indirectCommand);
    }

    void scalePoints() {
      const float scale = 0.0002f;
      for (glm::vec3& p : points) {
        p *= scale;
      }
    }

    void createGLBuffer() {
      glGenBuffers(1, &gl_points);
      glBindBuffer(GL_ARRAY_BUFFER, gl_points);
      glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

      glGenBuffers(1, &gl_signs);
      glBindBuffer(GL_ARRAY_BUFFER, gl_signs);
      glBufferData(GL_ARRAY_BUFFER, signs.size() * sizeof(glm::int32), signs.data(), GL_STATIC_DRAW);

      glGenBuffers(1, &gl_indirect_draw_commands);
      glBindBuffer(GL_DRAW_INDIRECT_BUFFER, gl_indirect_draw_commands);
      glBufferData(GL_DRAW_INDIRECT_BUFFER, indirectDrawCommands.size() * sizeof(DrawArraysIndirectCommand), indirectDrawCommands.data(), GL_STATIC_DRAW);
    }

    /// draws the buffers
    void draw(glm::mat4 mvp) {
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, gl_points);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, nullptr);

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, gl_signs);
      glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, NULL, nullptr);

      glBindBuffer(GL_DRAW_INDIRECT_BUFFER, gl_indirect_draw_commands);

      glUseProgram(gl_shader_program);
      glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&mvp);

      glMultiDrawArraysIndirect(GL_TRIANGLE_FAN, nullptr, static_cast<GLsizei>(indirectDrawCommands.size()), 0);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
    }

  };
};

#endif // __SHAPES_H__