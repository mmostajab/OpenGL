#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


// STD
#include <string>
#include <stdlib.h>
#include <stdio.h>

typedef  struct {
  GLuint  count;
  GLuint  instanceCount;
  GLuint  firstIndex;
  GLuint  baseVertex;
  GLuint  baseInstance;
} DrawElementsIndirectCommand;

class Application {
public:
  Application(const std::string& dataset_filename, const float& minRange = 0.0f, const float& maxRange = 10.0f, const float& minAlpha = 0.2f, const float& maxAlpha = 1.0f, const float& stepsize = 0.001f, const float& rangestep = 0.0001f);

  void init(const unsigned int& width, const unsigned int& height);
  void init();
  void run();
  void shutdown();

  ~Application();

private:
  void create();
  void update(float time, float elapsedTime);
  void draw();
  void draw_controlpoints();
  void draw_bspline();

  void compileShaders();

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

private:
  static GLFWwindow* m_window;
  glm::mat4 m_projmat, m_viewmat, m_worldmat, m_inv_viewmat;
  glm::vec3 m_cam_pos;
  static unsigned int m_width, m_height;

  // application-specific data
  GLuint shader, bspline_shader;
  GLuint vertex_buffer;
  GLuint color_buffer; 
  int n_control_points;
  int bspline_num_draws;

  GLuint bspline_vertex_buffer;
  GLuint bspline_control0_buffer;
  GLuint bspline_control1_buffer;
  GLuint bspline_controlpoints_buffer;
  static int    bspline_detail;
  static bool   show_controlpoints;

  bool initialization_step;
};

#endif