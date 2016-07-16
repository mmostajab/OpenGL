#ifndef __GRAPHICS_CYLINDER_H__
#define __GRAPHICS_CYLINDER_H__

// STD
#include <vector>
#include <stdint.h>

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>

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
			gl_shader_program = 0;
			gl_centers_buffer = 0;
			gl_radiuses_buffer = 0;
			gl_heights_buffer = 0;
			gl_signs_buffer = 0;

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

			gl_shader_program = 0;
			gl_centers_buffer = 0;
			gl_radiuses_buffer = 0;
			gl_heights_buffer = 0;
			gl_signs_buffer = 0;
		}

		/// adds a cylinder to the CPU side buffer
		void addCylinder(const glm::vec3& center, const glm::float32& radius, const glm::float32& height, int sign = 1) {
			const float scale = 1.0f;
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
			glBufferData(GL_ARRAY_BUFFER, centers_buffer.size() * sizeof(glm::vec3), centers_buffer.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, gl_radiuses_buffer);
			glBufferData(GL_ARRAY_BUFFER, radiuses_buffer.size() * sizeof(glm::float32), radiuses_buffer.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, gl_heights_buffer);
			glBufferData(GL_ARRAY_BUFFER, heights_buffer.size() * sizeof(glm::float32), heights_buffer.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, gl_signs_buffer);
			glBufferData(GL_ARRAY_BUFFER, signs_buffer.size() * sizeof(glm::int32), signs_buffer.data(), GL_STATIC_DRAW);

		}

		/// draws the buffers
		void draw(glm::mat4 mvp) {
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
			glBindBuffer(GL_ARRAY_BUFFER, gl_heights_buffer);
			glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, NULL, nullptr);

			glUseProgram(gl_shader_program);
			glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&mvp);

			glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(centers_buffer.size()));

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
		}
	};
}

#endif // __GRAPHICS_CYLINDER_H__