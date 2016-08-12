#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "helper.h"

#include <Windows.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <gl/glew.h>
//#include <../src/glew.c>
#include <GLFW/glfw3.h>
#include <GLFW\glfw3native.h>
#include <glm/glm.hpp>


// STD
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <complex>

#include "Camera.h"

#define MAX_FRAMEBUFFER_WIDTH 2048
#define MAX_FRAMEBUFFER_HEIGHT 2048

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
	int    nSegs  = -1;

	ArcSegment(
		glm::vec3 _p1 = glm::vec3(1.0f, 0.0f, 0.0f), 
		glm::vec3 _p2 = glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3 _center = glm::vec3(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/, 
		int _nSegs = 5) {

		p1		= _p1;
		p2		= _p2;
		center	= _center;
		//alpha = _alpha;
		createBuffer(_nSegs);
	}

	void createBuffer(int _nSegs) {
		
		if (_nSegs == nSegs) return;

		// std::cout << "Number of segments changed from " << nSegs << " to " << _nSegs << std::endl;

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
				sinf(thetha)         / sinf(static_cast<float>(alpha)) * b;
			
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

		float radius = glm::max( glm::length(p1_proj - center_proj) * w, glm::length(p2_proj - center_proj) * h ); //glm::length(len) / (2 * sin(static_cast<float>(alpha)));
		float alpha = 
			acosf(glm::dot(p1_proj - center_proj, p2_proj - center_proj) 
				/ (glm::length(p1_proj - center_proj) * glm::length(p1_proj - center_proj)));

		int curve_length = static_cast<int>(radius * static_cast<float>(alpha)) + 1;
		createBuffer(curve_length);

	}

	void drawBuffer() {

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

		glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

		glDisableVertexAttribArray(0);

	}
};

class Application {
public:
    Application();

	void init(const unsigned int& width, const unsigned int& height, HGLRC mainWindowContext);
	void init(const unsigned int& width, const unsigned int& height);
    void init();
    void run();
	void run_onestep();
    void shutdown();

    ~Application();

// private:
public:
    void create();
    void update(float time, float elapsedTime);
    void draw();

    void compileShaders();

    void prepare_framebuffer();

    GLuint simple_program;

private:
    static void EventMouseButton(GLFWwindow* window, int button, int action, int mods);
    static void EventMousePos(GLFWwindow* window, double xpos, double ypos);
    static void EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset);
    static void EventKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void EventChar(GLFWwindow* window, int codepoint);

    // Callback function called by GLFW when window size changes
    static void WindowSizeCB(GLFWwindow* window, int width, int height);
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static bool m_controlKeyHold;
    static bool m_altKeyHold;
    static bool m_w_pressed, m_s_pressed, m_a_pressed, m_d_pressed, m_q_pressed, m_e_pressed;
    static bool m_mouse_left_drag, m_mouse_middle_drag, m_mouse_right_drag;

//private:
public:
    static GLFWwindow* m_window;
    glm::mat4 m_projmat, m_viewmat, m_worldmat, m_inv_viewmat, m_mvp_mat;
    static unsigned int m_width, m_height;
    GLuint m_coord_system_program;
    GLuint m_transformation_buffer, m_lighting_buffer, m_general_buffer;
   
	// scene objects
	static Camera m_camera;

    // Rendering buffer
    static int  rendering_state;
    GLuint      render_fbo;
    GLuint      fbo_textures[3];
    GLuint      quad_vao;

	std::vector<ArcSegment> arcSegments;
};

#endif