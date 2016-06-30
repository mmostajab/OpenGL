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

// Static Members
GLFWwindow*				    Application::m_window             = 0;
unsigned int			    Application::m_width              = 0;
unsigned int			    Application::m_height             = 0;
bool					        Application::m_controlKeyHold     = false;
bool					        Application::m_altKeyHold         = false;
bool					        Application::m_w_pressed          = false;
bool					        Application::m_s_pressed          = false;
bool					        Application::m_a_pressed          = false;
bool					        Application::m_d_pressed          = false;
bool					        Application::m_q_pressed          = false;
bool					        Application::m_e_pressed          = false;
bool                  Application::m_mouse_left_drag    = false;
bool                  Application::m_mouse_middle_drag  = false;
bool                  Application::m_mouse_right_drag   = false;
int                   Application::bspline_detail       = 8;
bool                  Application::show_controlpoints   = true;

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


static GLuint compile_link_vs_fs(const std::string& vert_shader_file, const std::string& geom_shader_file, const std::string& frag_shader_file) {
  GLuint vertex_shader, geom_shader, fragment_shader;
  std::string vertex_shader_source = convertFileToString(vert_shader_file);
  std::string geometry_shader_source = convertFileToString(geom_shader_file);
  std::string coord_sys_shader_fragment_source = convertFileToString(frag_shader_file);
  

  if (vertex_shader_source.size() == 0 || geometry_shader_source.size() == 0 || coord_sys_shader_fragment_source.size() == 0){
    std::cout << "Problem in finding " << vert_shader_file << " or " << geom_shader_file << " or " << frag_shader_file << std::endl;
    return -1;
  }


  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar* vertex_shader_sourcePtr = vertex_shader_source.c_str();
  const GLchar* fragment_shader_sourcePtr = coord_sys_shader_fragment_source.c_str();
  const GLchar* geom_shader_sourcePtr = geometry_shader_source.c_str();

  glShaderSource(vertex_shader, 1, &vertex_shader_sourcePtr, NULL);
  glShaderSource(geom_shader, 1, &geom_shader_sourcePtr, NULL);
  glShaderSource(fragment_shader, 1, &fragment_shader_sourcePtr, NULL);

  glCompileShader(vertex_shader);
  glCompileShader(geom_shader);
  glCompileShader(fragment_shader);

  show_compiler_error(vertex_shader);
  show_compiler_error(geom_shader);
  show_compiler_error(fragment_shader);

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, geom_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgramARB(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(geom_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}



Application::Application(const std::string& dataset_filename, const float& minRange, const float& maxRange, const float& minAlpha, const float& maxAlpha, const float& stepsize, const float& rangestep) {
  initialization_step = true;
  m_worldmat = m_viewmat = m_projmat = glm::mat4(1.0f);
}

void Application::init(const unsigned int& width, const unsigned int& height) {

  m_width = width; m_height = height;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  m_window = glfwCreateWindow(width, height, "B-Splines: Beta", NULL, NULL);
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
  srand(clock());
  const int num_points = 19;
  std::vector<glm::vec3> vertex_positions, vertex_colors;

  std::vector<glm::vec3> bspline_positions, bspline_control0_points, bspline_control1_points;

  for (int i = -(num_points - 1)/2; i <= (num_points - 1)/2; i++){

    glm::vec3 bspline_point;
    if (i == -(num_points - 1) / 2 + 1)
      bspline_point = glm::vec3(i-1, 2 * cos(i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
    else if (i == (num_points - 1) / 2 - 1)
      bspline_point = glm::vec3(i + 1, -2 * cos(i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
    else
      bspline_point = glm::vec3(i, 4 * sin(i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);

    vertex_positions.push_back(bspline_point);
    vertex_colors.push_back(glm::vec3(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f));

    //if ((i + (num_points - 1) / 2) % 3 == 0) bspline_positions.push_back(bspline_point);
    //if ((i + (num_points - 1) / 2) % 3 == 1) bspline_control0_points.push_back(bspline_point);
    //if ((i + (num_points - 1) / 2) % 3 == 2) bspline_control1_points.push_back(bspline_point);

  }

  /*vertex_positions.push_back(glm::vec3(+2, -2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(+0, -0, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(-2, 2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(+2, 2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(+0, 0, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(-2, -2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));*/

  for (size_t i = 0; i < vertex_positions.size() - 3; i++){
    bspline_positions.push_back(vertex_positions[i]);
    bspline_positions.push_back(vertex_positions[i+3]);
    bspline_control0_points.push_back(vertex_positions[i + 1]);
    bspline_control0_points.push_back(vertex_positions[i + 1]);
    bspline_control1_points.push_back(vertex_positions[i + 2]);
    bspline_control1_points.push_back(vertex_positions[i + 2]);
  }

  //n_control_points = num_points;
  n_control_points = num_points;
  bspline_num_draws = static_cast<int>(bspline_positions.size());
  
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(glm::vec3), vertex_positions.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &color_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(glm::vec3), vertex_colors.data(), GL_STATIC_DRAW);

  // bspline buffers
  glGenBuffers(1, &bspline_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, bspline_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, bspline_positions.size() * sizeof(glm::vec3), bspline_positions.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &bspline_control0_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, bspline_control0_buffer);
  glBufferData(GL_ARRAY_BUFFER, bspline_control0_points.size() * sizeof(glm::vec3), bspline_control0_points.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &bspline_control1_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, bspline_control1_buffer);
  glBufferData(GL_ARRAY_BUFFER, bspline_control1_points.size() * sizeof(glm::vec3), bspline_control1_points.data(), GL_STATIC_DRAW);
}

void Application::update(float time, float timeSinceLastFrame) {
  //updateBSpline(time);
  float v = (float)clock() / 3000.0f * glm::pi<float>();
  float r = 0.0f;
  //m_cam_pos = glm::vec3(r * sin(v), r, r * cos(v));
  m_cam_pos = glm::vec3(r * sin(v), r * cos(v), 13);
  m_viewmat = glm::lookAt(m_cam_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  m_inv_viewmat = glm::inverse(m_viewmat);
  m_projmat = glm::perspective(glm::pi<float>() / 3.0f, (float)m_width / m_height, 0.1f, 1000.0f);
}

void Application::updateBSpline(float time){
  const int num_points = 19;
  std::vector<glm::vec3> vertex_positions, vertex_colors;

  std::vector<glm::vec3> bspline_positions, bspline_control0_points, bspline_control1_points;
  int animType = 1;
  for (int i = -(num_points - 1) / 2; i <= (num_points - 1) / 2; i++){

    glm::vec3 bspline_point;

    if (animType == 0){
      if (i == -(num_points - 1) / 2 + 1)
        bspline_point = glm::vec3(i - 1, 2 * cos(time + i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
      else if (i == (num_points - 1) / 2 - 1)
        bspline_point = glm::vec3(i + 1, -2 * cos(time + i * glm::pi<float>() / 2.0f), 0.5f/* + i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
      else
        bspline_point = glm::vec3(i, 4 * sin(time + i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
    }
    else if (animType == 1){
      if (i == -(num_points - 1) / 2 + 1)
        bspline_point = glm::vec3(i - 1, 2 * cos(time * i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
      else if (i == (num_points - 1) / 2 - 1)
        bspline_point = glm::vec3(i + 1, -2 * cos(time * i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
      else
        bspline_point = glm::vec3(i, 4 * sin(time * i * glm::pi<float>() / 2.0f), 0.5f /*+ i % 2 == 0 ? i * i / 6.0f : -i * i / 6.0f*/);
    }

    vertex_positions.push_back(bspline_point);
    vertex_colors.push_back(glm::vec3(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f));

    //if ((i + (num_points - 1) / 2) % 3 == 0) bspline_positions.push_back(bspline_point);
    //if ((i + (num_points - 1) / 2) % 3 == 1) bspline_control0_points.push_back(bspline_point);
    //if ((i + (num_points - 1) / 2) % 3 == 2) bspline_control1_points.push_back(bspline_point);

  }

  /*vertex_positions.push_back(glm::vec3(+2, -2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(+0, -0, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(-2, 2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(+2, 2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(+0, 0, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));
  vertex_positions.push_back(glm::vec3(-2, -2, 0.0f));
  vertex_colors.push_back(glm::vec3(1.0f));*/

  for (size_t i = 0; i < vertex_positions.size() - 3; i++){
    bspline_positions.push_back(vertex_positions[i]);
    bspline_positions.push_back(vertex_positions[i + 3]);
    bspline_control0_points.push_back(vertex_positions[i + 1]);
    bspline_control0_points.push_back(vertex_positions[i + 1]);
    bspline_control1_points.push_back(vertex_positions[i + 2]);
    bspline_control1_points.push_back(vertex_positions[i + 2]);
  }

  //n_control_points = num_points;
  n_control_points = num_points;
  bspline_num_draws = static_cast<int>(bspline_positions.size());

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_positions.size() * sizeof(glm::vec3), vertex_positions.data());

  //glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  //glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_positions.size() * sizeof(glm::vec3), vertex_colors.data());

  // bspline buffers
  glBindBuffer(GL_ARRAY_BUFFER, bspline_vertex_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, bspline_positions.size() * sizeof(glm::vec3), bspline_positions.data());

  glBindBuffer(GL_ARRAY_BUFFER, bspline_control0_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, bspline_control0_points.size() * sizeof(glm::vec3), bspline_control0_points.data());

  glBindBuffer(GL_ARRAY_BUFFER, bspline_control1_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, bspline_control1_points.size() * sizeof(glm::vec3), bspline_control1_points.data());
}

void Application::draw_controlpoints() {
  GLenum e;

  glUseProgram(shader);
  e = glGetError();

  GLint proj_mat = glGetUniformLocation(shader, "proj_mat");
  GLint view_mat = glGetUniformLocation(shader, "view_mat");
  GLint inv_view_mat = glGetUniformLocation(shader, "inv_view_mat");
  GLint world_mat = glGetUniformLocation(shader, "world_mat");
  GLint cam_pos = glGetUniformLocation(shader, "camera_pos");

  m_worldmat = glm::mat4(1.0f);
  glUniformMatrix4fv(proj_mat, 1, GL_FALSE, &m_projmat[0][0]);
  glUniformMatrix4fv(inv_view_mat, 1, GL_FALSE, &m_inv_viewmat[0][0]);
  glUniformMatrix4fv(view_mat, 1, GL_FALSE, &m_viewmat[0][0]);
  glUniformMatrix4fv(world_mat, 1, GL_FALSE, &m_worldmat[0][0]);
  //std::cout << "Camera Position = " << m_cam_pos.x << " " << m_cam_pos.y << " " << m_cam_pos.z << std::endl;
  glUniform3fv(cam_pos, 1, (float*)&m_cam_pos);

  e = glGetError();
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, 0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NULL, 0);
  e = glGetError();

  glPointSize(10);
  glDrawArrays(GL_POINTS, 0, n_control_points);
  glDrawArrays(GL_LINE_STRIP, 0, n_control_points);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Application::draw_bspline() {
  GLenum e = glGetError();
  e = glGetError();
  glUseProgram(bspline_shader);
  e = glGetError();

  GLint proj_mat = glGetUniformLocation(bspline_shader, "proj_mat");
  GLint view_mat = glGetUniformLocation(bspline_shader, "view_mat");
  GLint inv_view_mat = glGetUniformLocation(bspline_shader, "inv_view_mat");
  GLint world_mat = glGetUniformLocation(bspline_shader, "world_mat");
  GLint cam_pos = glGetUniformLocation(bspline_shader, "camera_pos");

  GLint bsplineDetail = glGetUniformLocation(bspline_shader, "curve_detail");
  
  m_worldmat = glm::mat4(1.0f);
  glUniformMatrix4fv(proj_mat, 1, GL_FALSE, &m_projmat[0][0]);
  glUniformMatrix4fv(inv_view_mat, 1, GL_FALSE, &m_inv_viewmat[0][0]);
  glUniformMatrix4fv(view_mat, 1, GL_FALSE, &m_viewmat[0][0]);
  glUniformMatrix4fv(world_mat, 1, GL_FALSE, &m_worldmat[0][0]);
  //std::cout << "Camera Position = " << m_cam_pos.x << " " << m_cam_pos.y << " " << m_cam_pos.z << std::endl;
  glUniform3fv(cam_pos, 1, (float*)&m_cam_pos);
  glUniform1i(bsplineDetail, bspline_detail);

  e = glGetError();
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, bspline_vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, 0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, bspline_control0_buffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NULL, 0);
  e = glGetError();

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, bspline_control1_buffer);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, NULL, 0);
  e = glGetError();

  glDrawArrays(GL_LINES, 0, bspline_num_draws);

  e = glGetError();
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
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
  
  if (show_controlpoints) draw_controlpoints();

  draw_bspline();
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

    update(static_cast<float>(elapsed_since_start), static_cast<float>(elapsed_since_last_frame));
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
  shader = compile_link_vs_fs("../../src/glsl/vertex.glsl", "../../src/glsl/fragment.glsl"); 
  bspline_shader = compile_link_vs_fs("../../src/glsl/bspline_vert.glsl", "../../src/glsl/bspline_geom.glsl", "../../src/glsl/bspline_frag.glsl");
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

  if (action == GLFW_PRESS){
    if (key == GLFW_KEY_W) bspline_detail++;
    if (key == GLFW_KEY_S && bspline_detail > 2) bspline_detail--;
    if (key == GLFW_KEY_H) show_controlpoints = !show_controlpoints;
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
