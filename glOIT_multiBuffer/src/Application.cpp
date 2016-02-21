#ifdef WIN32
	#include <Windows.h>
#endif

#include "application.h"
#include "plydatareader.h"

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
int                   Application::rendering_state      = 0;
Camera				        Application::m_camera;
float                 Application::transparency_value   = 0.5f;
int                   Application::comp_shaders            = 1;

Application::Application() {
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

    GLenum e = glGetError();
    //glEnable(GL_DEPTH);
    e = glGetError();
    glEnable(GL_DEPTH_TEST);
    e = glGetError();
}

void Application::init() {
    GLenum e = glGetError();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    e = glGetError();

    m_camera.SetMode(MODELVIEWER);
    //m_camera.SetMode(FREE);
    m_camera.SetPosition(glm::vec3(3.0f, 3.0f, 3.0f));
    m_camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera.SetClipping(0.01f, 100.0f);
    m_camera.SetFOV(60);
    m_camera.SetViewport(0, 0, m_width, m_height);
    m_camera.camera_scale = 0.01f;

    prepare_framebuffer();
    prepare_Order_Independent_Transparency();

    glGenBuffers(1, &m_transformation_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_transformation_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_lighting_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_lighting_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_general_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_general_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
}

void Application::create() {
  compileShaders();

  int dim = static_cast<int>(glm::ceil(glm::sqrt(NUM_FRAME_BUFFERS)));

  for (int idx = 0; idx < NUM_FRAME_BUFFERS; idx++){

    float width = 400.0f;
    glm::vec3 offset((idx % dim) / static_cast<float>(dim), 0.0f, idx / static_cast<float>(dim));
    glm::float32 offset_scale = 20.0f;

    PlyDataReader::getSingletonPtr()->renew();

#define aaa
#ifdef aaa
    PlyDataReader::getSingletonPtr()->readDataInfo("big_porsche.ply", nullptr, 0);
#else
    PlyDataReader::getSingletonPtr()->readDataInfo("happy.ply", nullptr, 0);
#endif

    unsigned int nVertices = PlyDataReader::getSingletonPtr()->getNumVertices();
    unsigned int nFaces = PlyDataReader::getSingletonPtr()->getNumFaces();


    vertices[idx].resize(nVertices + 4);
    indices[idx].resize(nFaces * 3 + 6);

    PlyDataReader::getSingletonPtr()->readData(vertices[idx].data(), indices[idx].data());

    glm::vec3 center;
    glm::float32 min_y = vertices[idx][0].pos.y;
    size_t i = 0;
    for (; i < vertices[idx].size() - 4; i++) {
      center += vertices[idx][i].pos + offset_scale * offset;
      min_y = glm::min(min_y, vertices[idx][i].pos.y);
    }
    center /= vertices[idx].size();

    vertices[idx][nVertices + 0].pos = glm::vec3(-width, min_y, -width) + offset_scale * offset;
    vertices[idx][nVertices + 0].normal = glm::vec3(0, 1, 0);
    vertices[idx][nVertices + 1].pos = glm::vec3(-width, min_y, width) + offset_scale * offset;
    vertices[idx][nVertices + 1].normal = glm::vec3(0, 1, 0);
    vertices[idx][nVertices + 2].pos = glm::vec3(width, min_y, -width) + offset_scale * offset;
    vertices[idx][nVertices + 2].normal = glm::vec3(0, 1, 0);
    vertices[idx][nVertices + 3].pos = glm::vec3(width, min_y, width) + offset_scale * offset;
    vertices[idx][nVertices + 3].normal = glm::vec3(0, 1, 0);

    indices[idx][3 * nFaces + 0] = nVertices + 0;
    indices[idx][3 * nFaces + 1] = nVertices + 1;
    indices[idx][3 * nFaces + 2] = nVertices + 2;
    indices[idx][3 * nFaces + 3] = nVertices + 2;
    indices[idx][3 * nFaces + 4] = nVertices + 1;
    indices[idx][3 * nFaces + 5] = nVertices + 3;

    for (size_t i = 0; i < vertices[idx].size(); i++) {
      vertices[idx][i].pos -= center;
    }

    for (size_t i = 0; i < vertices[idx].size(); i++) {

      //vertices[i].pos *= 30.0;
#ifdef aaa
      vertices[idx][i].pos *= 0.4;
#else
      vertices[i].pos *= 1.0;
#endif
    }

    glGenBuffers(1, &vertices_buffer[idx]);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer[idx]);
    glBufferData(GL_ARRAY_BUFFER, vertices[idx].size() * sizeof(PlyObjVertex), vertices[idx].data(), GL_STATIC_DRAW);

    glGenBuffers(1, &indices_buffer[idx]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer[idx]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[idx].size() * sizeof(unsigned int), indices[idx].data(), GL_STATIC_DRAW);
  }
}

void Application::update(float time, float timeSinceLastFrame) {

  if (comp_shaders == 1){
    compileShaders();
    comp_shaders = 0;
  }

    if (m_w_pressed)
        m_camera.Move(CameraDirection::FORWARD);

    if (m_s_pressed)
        m_camera.Move(CameraDirection::BACK);

    if (m_a_pressed)
        m_camera.Move(CameraDirection::LEFT);

    if (m_d_pressed)
        m_camera.Move(CameraDirection::RIGHT);

    if (m_q_pressed)
        m_camera.Move(CameraDirection::UP);

    if (m_e_pressed)
        m_camera.Move(CameraDirection::DOWN);

    // Updating the camera matrices
    m_camera.Update();
    m_camera.GetMatricies(m_projmat, m_viewmat, m_worldmat);
    m_inv_viewmat = glm::inverse(m_viewmat);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_transformation_buffer);
    glm::mat4* transform_matrices = (glm::mat4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 3 * sizeof(glm::mat4), GL_MAP_WRITE_BIT);
    transform_matrices[0] = m_projmat;
    transform_matrices[1] = m_viewmat;
    transform_matrices[2] = m_worldmat;
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    // updating the lighting info
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_lighting_buffer);
    glm::vec4* light_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
    light_info[0] = glm::vec4(-1, -1, -1, 0);
    light_info[1] = glm::vec4(m_camera.getPosition(), 1.0f);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    // Buffer 2 is reserved for the sample points of the Ambient Occlusion Rendering

    // updating the general information for every object
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_general_buffer);
    glm::vec4* general_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void Application::draw() {

  glViewport(0, 0, m_width, m_height);

  for (int i = 0; i < NUM_FRAME_BUFFERS; i++){
    glBindFramebuffer(GL_FRAMEBUFFER, render_fbo[i]);
    glEnable(GL_DEPTH_TEST);

    float back_color[] = { 1, 1, 1, 1 };
    float zero[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    float one = 1.0f;

    glClearBufferfv(GL_COLOR, 0, back_color);
    glClearBufferfv(GL_COLOR, 1, zero);
    glClearBufferfv(GL_DEPTH, 0, &one);

    draw_framebuffer(i);
    draw_Order_Independent_Transparency(i);
  }
    //drawPly2();
  
  glBindFramebuffer(GL_FRAMEBUFFER, main_render_fbo);

  combine_Lists_Order_Independent_Transparency();

  apply_transparency();
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(ssao_program);
  GLint rendering_state_loc = glGetUniformLocation(ssao_program, "rendering_state");
  glUniform1i(rendering_state_loc, rendering_state);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, main_fbo_textures[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, main_fbo_textures[1]);
  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(quad_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
  // Draw the world coordinate system
  glViewport(0, 0, 100, 100);
  glUseProgram(m_coord_system_program);
  glDrawArrays(GL_LINES, 0, 6);
}

  void Application::drawPly(const int& idx) {

  

  bool use_const_color = false;
  float const_color[] = { 1.0f, 1.0f, 1.0f };
  if (use_const_color) {
    glUniform1i(5, 1);
    glUniform3fv(6, 1, const_color);
  }
  else {
    glUniform1i(5, 0);
  }

  //glUniform1i(0, 1);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  int stride = sizeof(PlyObjVertex);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer[idx]);
  glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, stride, 0);
  glVertexAttribPointer((GLint)1, 3, GL_FLOAT, GL_FALSE, stride, (char*)0 + 12);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer[idx]);

  //std::cout << "Try to draw...\n";
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices[idx].size()), GL_UNSIGNED_INT, 0);
  // std::cout << "Drawing done...\n";

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Application::drawPly2() {



  bool use_const_color = false;
  float const_color[] = { 1.0f, 1.0f, 1.0f };
  if (use_const_color) {
    glUniform1i(5, 1);
    glUniform3fv(6, 1, const_color);
  }
  else {
    glUniform1i(5, 0);
  }

  //glUniform1i(0, 1);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  int stride = sizeof(PlyObjVertex);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer2);
  glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, stride, 0);
  glVertexAttribPointer((GLint)1, 3, GL_FLOAT, GL_FALSE, stride, (char*)0 + 12);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer2);

  //std::cout << "Try to draw...\n";
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices2.size()), GL_UNSIGNED_INT, 0);
  // std::cout << "Drawing done...\n";

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
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
    double elapsed_since_start          = current_time - start_time;
    double elapsed_since_last_frame     = current_time - start_frame;

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
  ssao_program = compile_link_vs_fs("../../src/glsl/ssao.vert", "../../src/glsl/ssao.frag");
  m_coord_system_program = compile_link_vs_fs("../../src/glsl/coord_sys.vert", "../../src/glsl/coord_sys.frag");
  resolve_order_independence_program = compile_link_vs_fs("../../src/glsl/oit.vert", "../../src/glsl/oit.frag");
  combine_order_independence_lists_program = compile_link_vs_fs("../../src/glsl/combine_lists.vert", "../../src/glsl/combine_lists.frag");
  blend_order_independence_buffers_program = compile_link_vs_fs("../../src/glsl/blend.vert", "../../src/glsl/blend.frag");
  ply_program = compile_link_vs_fs("../../src/glsl/ply_oit.vert", "../../src/glsl/ply_oit.frag");
  //render_oreder_independece_linked_list_program = compile_link_vs_fs("../../src/glsl/OIT_build_list.vert", "../../src/glsl/OIT_build_list.frag");
  //resolve_order_independence_program = compile_link_vs_fs("../../src/glsl/OIT_resolve.vert", "../../src/glsl/OIT_resolve.frag");
}

void Application::prepare_framebuffer() {
  GLenum e;

  for (int i = 0; i < NUM_FRAME_BUFFERS; i++){
    glGenFramebuffers(1, &render_fbo[i]);
    glBindFramebuffer(GL_FRAMEBUFFER, render_fbo[i]);
    glGenTextures(3, fbo_textures[i]);

    glBindTexture(GL_TEXTURE_2D, fbo_textures[i][0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, 2048, 2048);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, fbo_textures[i][1]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 2048, 2048);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, fbo_textures[i][2]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 2048, 2048);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo_textures[i][0], 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, fbo_textures[i][1], 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fbo_textures[i][2], 0);

    static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

    glDrawBuffers(2, draw_buffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    e = glGetError();
    if (e != GL_NO_ERROR) std::cout << __LINE__ << " " << e << std::endl;

  }

  glGenFramebuffers(1, &main_render_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, main_render_fbo);
  glGenTextures(3, main_fbo_textures);

  glBindTexture(GL_TEXTURE_2D, main_fbo_textures[0]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, 2048, 2048);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, main_fbo_textures[1]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 2048, 2048);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, main_fbo_textures[2]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 2048, 2048);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, main_fbo_textures[0], 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, main_fbo_textures[1], 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, main_fbo_textures[2], 0);

  static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

  glDrawBuffers(2, draw_buffers);

  e = glGetError();
  if (e != GL_NO_ERROR) std::cout << __LINE__ << " " << e << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenVertexArrays(1, &quad_vao);
  glBindVertexArray(quad_vao);

  e = glGetError();
  if (e != GL_NO_ERROR) std::cout << __LINE__ << " " << e << std::endl;
}

void Application::prepare_Order_Independent_Transparency() {
  GLuint* data;

  // Create buffer for clearing the head pointer texture
  glGenBuffers(1, &head_pointer_clear_buffer);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint), NULL, GL_STATIC_DRAW);
  data = (GLuint *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  memset(data, 0x00, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint));
  glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  for (int i = 0; i < NUM_FRAME_BUFFERS; i++){
    // Create head pointer texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &head_pointer_texture[i]);
    glBindTexture(GL_TEXTURE_2D, head_pointer_texture[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindImageTexture(0, head_pointer_texture[i], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

    // Create the atomic counter buffer
    glGenBuffers(1, &atomic_counter_buffer[i]);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer[i]);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

    // Create the linked list storage buffer
    glGenBuffers(1, &linked_list_buffer[i]);
    glBindBuffer(GL_TEXTURE_BUFFER, linked_list_buffer[i]);
    glBufferData(GL_TEXTURE_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * 6 * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);

    // Bind it to a texture (for use as a TBO)
    glGenTextures(1, &linked_list_texture[i]);
    glBindTexture(GL_TEXTURE_BUFFER, linked_list_texture[i]);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, linked_list_buffer[i]);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    glBindImageTexture(1, linked_list_texture[i], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);
  }

  // Create head pointer texture
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &main_head_pointer_texture);
  glBindTexture(GL_TEXTURE_2D, main_head_pointer_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindImageTexture(0, main_head_pointer_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

  // Create the atomic counter buffer
  glGenBuffers(1, &main_atomic_counter_buffer);
  glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, main_atomic_counter_buffer);
  glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

  // Create the linked list storage buffer
  glGenBuffers(1, &main_linked_list_buffer);
  glBindBuffer(GL_TEXTURE_BUFFER, main_linked_list_buffer);
  glBufferData(GL_TEXTURE_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * 6 * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
  glBindBuffer(GL_TEXTURE_BUFFER, 0);

  // Bind it to a texture (for use as a TBO)
  glGenTextures(1, &main_linked_list_texture);
  glBindTexture(GL_TEXTURE_BUFFER, main_linked_list_texture);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, main_linked_list_texture);
  glBindTexture(GL_TEXTURE_BUFFER, 0);

  glBindImageTexture(1, main_linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);
}

void Application::draw_framebuffer(const int& idx) {
  // Reset atomic counter
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer[idx]);
  GLuint * data = (GLuint *)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
  data[0] = 0;
  glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

  // Clear head-pointer image
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
  glBindTexture(GL_TEXTURE_2D, head_pointer_texture[idx]);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  // Bind head-pointer image for read-write
  glBindImageTexture(0, head_pointer_texture[idx], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

  // Bind linked-list buffer for write
  glBindImageTexture(1, linked_list_texture[idx], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);
  //  glUseProgram(render_oreder_independece_linked_list_program);
  glUseProgram(ply_program);

  glUniform1f(0, transparency_value);
  glUniform1i(2, 0);

  // disable constant color
  glUniform1i(5, 0);

  drawPly(idx);
}

void Application::draw_Order_Independent_Transparency(const int& idx) {
  glDisable(GL_DEPTH_TEST);

  // Bind head-pointer image for read-write
  glBindImageTexture(0, head_pointer_texture[idx], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

  // Bind linked-list buffer for write
  glBindImageTexture(1, linked_list_texture[idx], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

  glBindVertexArray(quad_vao);
  glUseProgram(resolve_order_independence_program);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Application::combine_Lists_Order_Independent_Transparency() {
  glDisable(GL_DEPTH_TEST);

  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, main_atomic_counter_buffer);
  GLuint * data = (GLuint *)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
  data[0] = 0;
  glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

  // Clear head-pointer image
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
  glBindTexture(GL_TEXTURE_2D, main_head_pointer_texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  for (int i = 0; i < NUM_FRAME_BUFFERS; i++){
    // Bind head-pointer image for read-write
    glBindImageTexture(0, main_head_pointer_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

    // Bind linked-list buffer for write
    glBindImageTexture(1, main_linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

    // Bind head-pointer image for read-write
    glBindImageTexture(2, head_pointer_texture[i], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);

    // Bind linked-list buffer for write
    glBindImageTexture(4, linked_list_texture[i], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32UI);

    glBindVertexArray(quad_vao);
    glUseProgram(combine_order_independence_lists_program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

void Application::apply_transparency() {
  glDisable(GL_DEPTH_TEST);

  glBindImageTexture(0, head_pointer_texture[1], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);

  // Bind linked-list buffer for write
  glBindImageTexture(1, linked_list_texture[1], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32UI);

  glBindVertexArray(quad_vao);
  glUseProgram(blend_order_independence_buffers_program);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Application::EventMouseButton(GLFWwindow* window, int button, int action, int mods) {

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    m_mouse_left_drag   = true;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    m_mouse_left_drag   = false;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    m_mouse_right_drag  = true;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    m_mouse_right_drag  = false;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    m_mouse_middle_drag = true;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    m_mouse_middle_drag = false;
}

void Application::EventMousePos(GLFWwindow* window, double xpos, double ypos) {

  if (m_controlKeyHold){
    if (m_mouse_left_drag){
      m_camera.Move2D(static_cast<int>(xpos), static_cast<int>(ypos));
    }
    if (m_mouse_right_drag){
      m_camera.OffsetFrustum(static_cast<int>(xpos), static_cast<int>(ypos));
    }
  }

  /*if (m_mouse_right_drag){
    m_camera.moveForward(static_cast<int>(ypos));
  }*/

  m_camera.SetMousePos(static_cast<int>(xpos), static_cast<int>(ypos));
  /*if (m_altKeyHold){
      m_seeding_curve->getControlPoints();
  }*/
}

void Application::EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {
  m_camera.MoveForward(static_cast<int>(yoffset));
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
    if (key == GLFW_KEY_LEFT_ALT)               m_altKeyHold     = true;
  }

  if (action == GLFW_RELEASE){
    if (key == GLFW_KEY_W)  m_w_pressed = false;
    if (key == GLFW_KEY_S)  m_s_pressed = false;
    if (key == GLFW_KEY_A)  m_a_pressed = false;
    if (key == GLFW_KEY_D)  m_d_pressed = false;
    if (key == GLFW_KEY_Q)  m_q_pressed = false;
    if (key == GLFW_KEY_E)  m_e_pressed = false;

    if (key == GLFW_KEY_M) {
      rendering_state = ++rendering_state % 3;
    }

    if (key == GLFW_KEY_P)
      comp_shaders = 1;

    if (key == GLFW_KEY_O)
      transparency_value += 0.01f;
    if (key == GLFW_KEY_L)
      transparency_value -= 0.01f;

    if (key == GLFW_KEY_LEFT_CONTROL)           m_controlKeyHold    = false;
    if (key == GLFW_KEY_LEFT_ALT)               m_altKeyHold        = false;

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    m_camera.SetMousePos(static_cast<int>(xpos), static_cast<int>(ypos));
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
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS){
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    m_camera.Move2D(static_cast<int>(xpos), static_cast<int>(ypos));
    m_controlKeyHold = true;
  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE){
    m_controlKeyHold = false;
  }
}