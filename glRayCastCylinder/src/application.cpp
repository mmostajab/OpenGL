#ifdef WIN32
#include <Windows.h>
#endif

#include "application.h"
#include "helper.h"

// STD
#include <iostream>
#include <fstream>
#include <time.h>
#include <random>

// GL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>


// Static Members
GLFWwindow*				    Application::m_window = 0;
unsigned int			    Application::m_width = 0;
unsigned int			    Application::m_height = 0;
bool					        Application::m_controlKeyHold = false;
bool					        Application::m_altKeyHold = false;
bool					        Application::m_w_pressed = false;
bool					        Application::m_s_pressed = false;
bool					        Application::m_a_pressed = false;
bool					        Application::m_d_pressed = false;
bool					        Application::m_q_pressed = false;
bool					        Application::m_e_pressed = false;
bool                  Application::m_mouse_left_drag = false;
bool                  Application::m_mouse_middle_drag = false;
bool                  Application::m_mouse_right_drag = false;

Application::Application() {
  initialization_step = true;
  m_worldmat = m_viewmat = m_projmat = glm::mat4(1.0f);
}

void Application::init(const unsigned int& width, const unsigned int& height) {

  m_width = width; m_height = height;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  m_window = glfwCreateWindow(width, height, "Stream Surface Generator (Demo): Beta", NULL, NULL);
  if (!m_window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(m_window);

  glfwSetKeyCallback(m_window, key_callback);
  glfwSetWindowSizeCallback(m_window, WindowSizeCB);
  glfwSetCursorPosCallback(m_window, EventMousePos);
  glfwSetScrollCallback(m_window, EventMouseWheel);
  glfwSetMouseButtonCallback(m_window, (GLFWmousebuttonfun)EventMouseButton);
  glfwSetKeyCallback(m_window, (GLFWkeyfun)EventKey);

  // - Directly redirect GLFW char events to AntTweakBar
  glfwSetCharCallback(m_window, (GLFWcharfun)EventChar);

  if (glewInit() != GLEW_OK){
    std::cout << "cannot intialize the glew.\n";
    exit(EXIT_FAILURE);
  }

  init();
}

void Application::init() {
  GLenum e = glGetError();
  glClearColor(19.0f / 255.0f, 9.0f / 255.0f, 99.0f / 255.0f, 1.0f);
  e = glGetError();

  e = glGetError();
  glEnable(GL_DEPTH_TEST);
  e = glGetError();
}

void Application::create() {
  compileShaders();


  //glCylinders.addCylinder(glm::vec3(0.0f, 0.0f, 0.0f), 0.25f, 1.0f, +1);

  int grid_size[2] = { 25, 25 };
  for (int i = 0; i < grid_size[0]; i++)
    for (int j = 0; j < grid_size[1]; j++) {
      glCylinders.addCylinder(glm::vec3(i * 1.0f, 0.0f, j * 1.0f), 0.25f, 1.0f, +1);
      glCylinders.addCylinder(glm::vec3(i * 1.0f, 0.0f, j * 1.0f), 0.25f, 1.0f, -1);
    }
  
  glCylinders.createGLBuffer();
  glCylinders.gl_shader_program =

//#define CYLINDER_AS_POINTS
#define CYLINDER_AS_BOXES
//#define CYLINDER_TESSELATED

#if defined(CYLINDER_AS_POINTS)
	  compile_link_vs_fs(
		  "../../src/glsl/cylinder/vertex.shader",
		  "../../src/glsl/cylinder/fragment.shader"
		  );
#elif defined(CYLINDER_AS_BOXES)
	  compile_link_vs_gs_fs(
		  "../../src/glsl/cylinder/vertex.shader",
		  "../../src/glsl/cylinder/geometry.shader",
		  "../../src/glsl/cylinder/fragment.shader"
		  );
#elif defined(CYLINDER_TESSELATED)
	  compile_link_vs_gs_ts_fs(
		  "../../src/glsl/cylinder/vertex.shader",
		  "../../src/glsl/cylinder/geometry.shader",
		  "../../src/glsl/cylinder/tessel_controlpoint.shader",
		  "../../src/glsl/cylinder/tessel_evaluate.shader",
		  "../../src/glsl/cylinder/fragment.shader"
		  );
#endif
}

void Application::update(float time, float timeSinceLastFrame) {
  float v = (float)clock() / 3000.0f * glm::pi<float>();
  m_eye     = 12.5f * glm::vec3(sin(v), cos(v), cos(v));
  m_inv_viewmat = glm::inverse(m_viewmat);
  m_viewmat = glm::lookAt(m_eye, glm::vec3(12.5f, 0.0f, 12.5f), glm::vec3(0.0f, 1.0f, 0.0f));
  m_projmat = glm::perspective(glm::pi<float>() / 3.0f, (float)m_width / m_height, 0.1f, 1000.0f);
}

void Application::draw() {
  GLenum e = glGetError();
  glViewport(0, 0, m_width, m_height);
  e = glGetError();
  float back_color[] = { 0.0f, 0.2f, 0.7f };
  float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  float one = 1.0f;
  e = glGetError();
  glClearBufferfv(GL_COLOR, 0, back_color);
  glClearBufferfv(GL_DEPTH, 0, &one);
  e = glGetError();
  glUseProgram(shader);
  e = glGetError();

  glFrontFace(GL_CW);
  //glFrontFace(GL_CCW);

#ifndef CULL_FACES
  glDisable(GL_CULL_FACE);
#else
  glEnable(GL_CULL_FACE);
  glCullFace(GL_NONE);
#endif
  glm::mat4 mvp = m_projmat * m_viewmat * m_worldmat;
  glCylinders.draw(m_eye, mvp);
}

void Application::run() {
  create();
  double start_time;
  double start_frame;
  start_time = start_frame = glfwGetTime();

  while (!glfwWindowShouldClose(m_window))
  {
    double frame_start_time = glfwGetTime();
    draw();
    double frame_end_time = glfwGetTime();


    glfwSwapBuffers(m_window);
    glfwPollEvents();

    double current_time = glfwGetTime();
    double elapsed_since_start = current_time - start_time;
    double elapsed_since_last_frame = current_time - start_frame;

    start_frame = glfwGetTime();

    update(static_cast<float>(elapsed_since_start), static_cast<float>( elapsed_since_last_frame ) );
  }
}

void Application::shutdown() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

Application::~Application() {
}

void Application::compileShaders() {
}

void Application::EventMouseButton(GLFWwindow* window, int button, int action, int mods) {
 
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    m_mouse_left_drag = true;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    m_mouse_left_drag = false;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    m_mouse_right_drag = true;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    m_mouse_right_drag = false;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    m_mouse_middle_drag = true;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    m_mouse_middle_drag = false;
}

void Application::EventMousePos(GLFWwindow* window, double xpos, double ypos) {
}

void Application::EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {

}

void Application::EventKey(GLFWwindow* window, int key, int scancode, int action, int mods) {

  if (action == GLFW_PRESS){
    if (m_controlKeyHold && key == GLFW_KEY_W)  m_w_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_S)  m_s_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_A)  m_a_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_D)  m_d_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_Q)  m_q_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_E)  m_e_pressed = true;

    if (key == GLFW_KEY_LEFT_CONTROL)           m_controlKeyHold = true;
    if (key == GLFW_KEY_LEFT_ALT)               m_altKeyHold = true;
  }

  if (action == GLFW_RELEASE){
    if (key == GLFW_KEY_W)  m_w_pressed = false;
    if (key == GLFW_KEY_S)  m_s_pressed = false;
    if (key == GLFW_KEY_A)  m_a_pressed = false;
    if (key == GLFW_KEY_D)  m_d_pressed = false;
    if (key == GLFW_KEY_Q)  m_q_pressed = false;
    if (key == GLFW_KEY_E)  m_e_pressed = false;

    if (key == GLFW_KEY_LEFT_CONTROL)           m_controlKeyHold = false;
    if (key == GLFW_KEY_LEFT_ALT)               m_altKeyHold = false;

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
  }
}

void Application::EventChar(GLFWwindow* window, int codepoint) {
}

// Callback function called by GLFW when window size changes
void Application::WindowSizeCB(GLFWwindow* window, int width, int height) {
  m_width = width; m_height = height;
  glViewport(0, 0, width, height);
}
void Application::error_callback(int error, const char* description) {
  fputs(description, stderr);
}
void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS){

  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE){
  }
}
