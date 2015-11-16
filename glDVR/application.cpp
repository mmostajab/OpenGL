#ifdef WIN32
#include <Windows.h>
#endif

#include "application.h"

// STD
#include <iostream>
#include <fstream>
#include <time.h>
#include <random>

// GL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#define WATER
//#define JOHANNES

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

// read the file content and generate a string from it.
static std::string convertFileToString(const std::string& filename) {
  std::ifstream ifile(filename);
  if (!ifile){
    return std::string("");
  }

  return std::string(std::istreambuf_iterator<char>(ifile), (std::istreambuf_iterator<char>()));

}

static void show_compiler_error(GLuint shader) {
  GLint isCompiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

    std::cout << "Shader " << shader << " Log: \n";
    std::cout << ((char*)&errorLog[0]);

    // Provide the infolog in whatever manor you deem best.
    // Exit with failure.
    glDeleteShader(shader); // Don't leak the shader.
    return;
  }
}

static GLuint compile_link_vs_fs(const std::string& vert_shader_file, const std::string& frag_shader_file) {
  GLuint vertex_shader, fragment_shader;
  std::string vertex_shader_source = convertFileToString(vert_shader_file);
  std::string coord_sys_shader_fragment_source = convertFileToString(frag_shader_file);

  if (vertex_shader_source.size() == 0 || coord_sys_shader_fragment_source.size() == 0){
    std::cout << "Problem in finding " << vert_shader_file << " or " << frag_shader_file << std::endl;
    return -1;
  }


  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar* vertex_shader_sourcePtr = vertex_shader_source.c_str();
  const GLchar* fragment_shader_sourcePtr = coord_sys_shader_fragment_source.c_str();

  glShaderSource(vertex_shader, 1, &vertex_shader_sourcePtr, NULL);
  glShaderSource(fragment_shader, 1, &fragment_shader_sourcePtr, NULL);

  glCompileShader(vertex_shader);
  glCompileShader(fragment_shader);

  show_compiler_error(vertex_shader);
  show_compiler_error(fragment_shader);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgramARB(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}


Application::Application(const std::string& dataset_filename, const float& minRange, const float& maxRange, const float& minAlpha, const float& maxAlpha, const float& stepsize) {
  initialization_step = true;
  m_worldmat = m_viewmat = m_projmat = glm::mat4(1.0f);

  m_dataset_filename = dataset_filename;
  m_minRange = minRange;
  m_maxRange = maxRange;
  m_minAlpha = minAlpha;
  m_maxAlpha = maxAlpha;
  m_stepSize = stepsize;
}

void Application::init(const unsigned int& width, const unsigned int& height) {

  m_width = width; m_height = height;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  m_window = glfwCreateWindow(width, height, "Direct Volume Rendering: Beta", NULL, NULL);
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

  glEnable(GL_BLEND);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}

void Application::create() {
  compileShaders();

  m_dims = glm::vec3(2.0f, 2.0f, 4.0f);

  const GLfloat vertex_positions[] = {
    -m_dims[0] / 2.0f, -m_dims[1] / 2.0f, -m_dims[2] / 2.0f,
    -m_dims[0] / 2.0f,  m_dims[1] / 2.0f, -m_dims[2] / 2.0f,
     m_dims[0] / 2.0f, -m_dims[1] / 2.0f, -m_dims[2] / 2.0f,
     m_dims[0] / 2.0f,  m_dims[1] / 2.0f, -m_dims[2] / 2.0f,

     m_dims[0] / 2.0f, -m_dims[1] / 2.0f,  m_dims[2] / 2.0f,
     m_dims[0] / 2.0f,  m_dims[1] / 2.0f,  m_dims[2] / 2.0f,
    -m_dims[0] / 2.0f, -m_dims[1] / 2.0f,  m_dims[2] / 2.0f,
    -m_dims[0] / 2.0f,  m_dims[1] / 2.0f,  m_dims[2] / 2.0f,
  };

  const GLfloat vertex_colors[] = {
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,

    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f
  };

  const GLfloat vertex_localPositions[] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
  };

  const GLuint vertex_indices[] = {
    // offset = 0
    // count  = 9
    0, 1, 2,
    2, 1, 3,
    2, 3, 4,
    // offset = 9
    // count  = 12
    4, 3, 5,
    4, 5, 6,
    6, 5, 7,
    6, 7, 0,
    // offset = 21
    // count  = 6
    0, 7, 1,
    6, 0, 2,
    // offset = 27
    // count  = 9
    2, 4, 6,
    7, 5, 3,
    7, 3, 1
  };

  DrawElementsIndirectCommand indirect_cmds[4] = {
    {
      9,
      1,
      0,
      0,
      0
    },
    {
      12,
      1,
      9,
      0,
      0
    },
    {
      6,
      1,
      21,
      0,
      0
    },
    {
      9,
      1,
      27,
      0,
      0
    },
  };

  glGenBuffers(1, &indirect_buffer);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);
  glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(indirect_cmds), indirect_cmds, GL_STATIC_DRAW);

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

  glGenBuffers(1, &color_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_colors), vertex_colors, GL_STATIC_DRAW);

  glGenBuffers(1, &localpos_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, localpos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_localPositions), vertex_localPositions, GL_STATIC_DRAW);

  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

  glGenTextures(1, (GLuint*)&datasetTex);

  glBindTexture(GL_TEXTURE_3D, datasetTex);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Convert the data to RGBA data.
  // Here we are simply putting the same value to R, G, B and A channels.
  // Usually for raw data, the alpha value
  // will be constructed by a threshold value given by the user 

  std::ifstream inFile(m_dataset_filename, std::ios::binary);

  if (!inFile){
    std::cout << "Cannot open " << m_dataset_filename << std::endl;
    exit(0);
  }

  float tex_dims[3];
  float val[3];
  inFile.read((char*)&tex_dims, 12);

  std::vector<float> data(tex_dims[0] * tex_dims[1] * tex_dims[2]);
  for (int idx = 0; idx < tex_dims[0] * tex_dims[1] * tex_dims[2]; ++idx)
  {

#ifndef TEST
#ifdef JOHANNES
    inFile.read((char*)&val, 12);
#else
    inFile.read((char*)&val, 12);
#endif
#else
    val[0] = (float)(idx % tex_dims[0]) / tex_dims[0] * 0.2f;
    val[1] = (float)((int)(idx / tex_dims[0]) % tex_dims[1]) / (float)(tex_dims[1]) * 0.2f;
    val[2] = (idx / (tex_dims[0] * tex_dims[1])) / (float)(tex_dims[2]) * 0.2f;
#endif

    data[idx] = (std::sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]));
  }

  glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F,
    tex_dims[0], tex_dims[1], tex_dims[2], 0,
    GL_RED, GL_FLOAT, (GLvoid *)data.data());
}

void Application::update(float time, float timeSinceLastFrame) {
  float v = (float)clock() / 3000.0f * glm::pi<float>();
  m_cam_pos = glm::vec3(4.0f * sin(v), 2.0f, 4.0f * cos(v));
  m_viewmat = glm::lookAt(m_cam_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  m_inv_viewmat = glm::inverse(m_viewmat);
  m_projmat = glm::perspective(glm::pi<float>() / 3.0f, (float)m_width / m_height, 0.1f, 1000.0f);

  if (m_w_pressed || m_s_pressed || m_a_pressed || m_d_pressed){
    if (m_w_pressed) m_minRange += 0.001f;
    if (m_s_pressed) m_minRange -= 0.001f;
    if (m_a_pressed) m_maxRange -= 0.001f;
    if (m_d_pressed) m_maxRange += 0.001f;
    std::cout << "Min = " << m_minRange << ", Max = " << m_maxRange << std::endl;
  }

  

}

void Application::draw() {
  GLenum e = glGetError();
  glViewport(0, 0, m_width, m_height);
  e = glGetError();
  float back_color[] = { 0, 0.2, 0.7 };
  float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  float one = 1.0f;
  e = glGetError();
  glClearBufferfv(GL_COLOR, 0, back_color);
  glClearBufferfv(GL_DEPTH, 0, &one);
  e = glGetError();
  glUseProgram(shader);
  e = glGetError();

  GLint proj_mat     = glGetUniformLocation(shader, "proj_mat");
  GLint view_mat     = glGetUniformLocation(shader, "view_mat");
  GLint inv_view_mat = glGetUniformLocation(shader, "inv_view_mat");
  GLint world_mat    = glGetUniformLocation(shader, "world_mat");
  GLint cam_pos      = glGetUniformLocation(shader, "camera_pos");
  GLint dims         = glGetUniformLocation(shader, "dims");
  GLint min_range_loc = glGetUniformLocation(shader, "min_range");
  GLint max_range_loc = glGetUniformLocation(shader, "max_range");

  m_worldmat = glm::mat4(1.0f);
  glUniformMatrix4fv(proj_mat,      1, GL_FALSE, &m_projmat[0][0]);
  glUniformMatrix4fv(inv_view_mat,  1, GL_FALSE, &m_inv_viewmat[0][0]);
  glUniformMatrix4fv(view_mat,      1, GL_FALSE, &m_viewmat[0][0]);
  glUniformMatrix4fv(world_mat,     1, GL_FALSE, &m_worldmat[0][0]);
  glUniform1i(glGetUniformLocation(shader, "tex"), 0);
  glUniform1f(min_range_loc, m_minRange);
  glUniform1f(max_range_loc, m_maxRange);
  //std::cout << "Camera Position = " << m_cam_pos.x << " " << m_cam_pos.y << " " << m_cam_pos.z << std::endl;
  glUniform3fv(cam_pos, 1, (float*)&m_cam_pos);
  glUniform3fv(dims, 1, (float*)&m_dims);

  e = glGetError();
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, 0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NULL, 0);
  e = glGetError();

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, localpos_buffer);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, NULL, 0);
  e = glGetError();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

  glBindTexture(GL_TEXTURE_3D, datasetTex);


#define TRY_INDIRECT
#ifndef TRY_INDIRECT
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
#else
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);
  glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, 4, 0);
#endif
  e = glGetError();

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
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

    update(elapsed_since_start, elapsed_since_last_frame);
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
  shader = compile_link_vs_fs("vertex.glsl", "fragment.glsl");
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
    if (/*m_controlKeyHold &&*/ key == GLFW_KEY_W)  m_w_pressed = true;
    if (/*m_controlKeyHold &&*/ key == GLFW_KEY_S)  m_s_pressed = true;
    if (/*m_controlKeyHold &&*/ key == GLFW_KEY_A)  m_a_pressed = true;
    if (/*m_controlKeyHold &&*/ key == GLFW_KEY_D)  m_d_pressed = true;
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
