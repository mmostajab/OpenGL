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

	bool									convex = true;
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
	  const float scale = 1.0f;// 0.0002f;
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
	GLboolean depth_test_status;
	GLboolean color_writemask;
	GLboolean depth_writemask;

	void beginStencilFill() {
		glGetBooleanv(GL_DEPTH_TEST,		&depth_test_status);
		glGetBooleanv(GL_COLOR_WRITEMASK,	&color_writemask);
		glGetBooleanv(GL_DEPTH_WRITEMASK,	&depth_writemask);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	}

	void endStencilFill() {
		if(depth_test_status)
			glEnable(GL_DEPTH_TEST);
		
		if(color_writemask)
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		if(depth_writemask)
			glDepthMask(GL_TRUE);
	}

	void drawPolygon(const glm::mat4 mvp) {
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

	void drawOutline(const glm::mat4 mvp) {
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, gl_points);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, gl_signs);
		glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, NULL, nullptr);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, gl_indirect_draw_commands);

		glUseProgram(gl_shader_program);
		glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&mvp);

		glMultiDrawArraysIndirect(GL_LINE_STRIP, nullptr, static_cast<GLsizei>(indirectDrawCommands.size()), 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

    void draw(glm::mat4 mvp) {

		if (convex) {
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			drawPolygon(mvp);
		}
		else {
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

			beginStencilFill();
				drawPolygon(mvp);
			endStencilFill();

			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			drawPolygon(mvp);
		}
    }

  };
};

#endif // __SHAPES_H__