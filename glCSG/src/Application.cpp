#ifdef WIN32
	#include <Windows.h>
#endif

#include "application.h"
#include "cst_reader.h"

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
GLFWwindow*			  Application::m_window				      = 0;
unsigned int		  Application::m_width              = 0;
unsigned int		  Application::m_height             = 0;
bool				      Application::m_controlKeyHold     = false;
bool				      Application::m_altKeyHold         = false;
bool				      Application::m_w_pressed          = false;
bool				      Application::m_s_pressed          = false;
bool				      Application::m_a_pressed          = false;
bool				      Application::m_d_pressed          = false;
bool				      Application::m_q_pressed          = false;
bool				      Application::m_e_pressed          = false;
bool              Application::m_mouse_left_drag    = false;
bool              Application::m_mouse_middle_drag  = false;
bool              Application::m_mouse_right_drag   = false;
int               Application::rendering_state      = 0;
Camera				    Application::m_camera;
float             Application::transparency_value   = 0.2f;
int               Application::comp_shaders         = 1;

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
    prepare_fragment_collecting();

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

void Application::loadCST() {
  std::vector<CSTPrimitive> cstPrimitives;
  readModelCST(cstPrimitives, "plasma.cst");

//#define SPLIT_POLYGONS
#ifdef  SPLIT_POLYGONS
  PolygonSplitter splitter(5, 1000, false);
  std::vector<CSTPrimitive> newcstPrimitives;
  for (auto& p : cstPrimitives)
    if (p.type == "POLYGON") {
      PolygonSplitter::Polygon polygon;

      for (auto &vertex : p.vertices)
      {
        PolygonSplitter::Point point;
        point.x = vertex.x;
        point.y = vertex.y;

        polygon.push_back(point);
      }

      PolygonSplitter::Polygons polygons = splitter.SplitPolygon(polygon);

      for (auto &polygon : polygons)
      {
        CSTPrimitive primitive = p;
        primitive.vertices.clear();

        AABB bbox;
        bbox.min = embree::Vec3f(+FLT_MAX);
        bbox.max = embree::Vec3f(-FLT_MAX);
        for (auto &point : polygon) {
          bbox.min = embree::min(bbox.min, embree::Vec3f());
          primitive.vertices.push_back(Vec2f(point.x, point.y));
        }

        newcstPrimitives.push_back(primitive);
      }
    }
    else {
      newcstPrimitives.push_back(p);
    }

    cstPrimitives = newcstPrimitives;
#endif

    int max_shapeId = 0;

    size_t n_cylinders = 0;
    size_t n_boxes = 0;
    size_t n_directedBoxes = 0;
    size_t n_polygons = 0;
    size_t n_polygon_points = 0;
    for (auto& p : cstPrimitives) {
      if (p.type == "CYLINDER")
        n_cylinders++;
      else if (p.type == "BOX")
        n_boxes++;
      else if (p.type == "DIRECTED_BOX")
        n_directedBoxes++;
      else if (p.type == "POLYGON") {
        n_polygons++;
        n_polygon_points += p.vertices.size();
      }

      max_shapeId = glm::max(max_shapeId, p.shape);
    }

    /* Log scene contents*/
    std::cout << "Number of cylinders = " << n_cylinders << std::endl;
    std::cout << "Number of boxes = " << n_boxes << std::endl;
    std::cout << "Number of directed boxes = " << n_directedBoxes << std::endl;
    std::cout << "Number of polygons = " << n_polygons << std::endl;
    std::cout << "Maximum shape Id = " << max_shapeId << std::endl;

    //g_scene = rtcDeviceNewScene(g_device, RTC_SCENE_STATIC, aflags);
    //createGroundPlane(g_scene);
    //g_cylinders = createAnalyticCylinders(g_scene, n_cylinders);
    //g_boxes = createAnalyticBoxes(g_scene, n_boxes);
    //g_directedBoxes = createAnalyticDirectedBoxes(g_scene, n_directedBoxes);
    //g_polygons = createAnalyticPolygons(g_scene, n_polygons);

    cylinders.resize(n_cylinders);
    boxes.resize(n_boxes);
    directedBoxes.resize(n_directedBoxes);
    polygons.resize(n_polygons);

    polygonPoints.resize(n_polygon_points);
    shapeColors.resize(max_shapeId + 1);

    size_t v_polygon = 0, i_polygon = 0;
    size_t c = 0, b = 0, d = 0, p = 0;
    for (size_t i = 0; i < cstPrimitives.size(); i++) {
      if (cstPrimitives[i].type == "CYLINDER") {
        cylinders[c].set(cstPrimitives[i].bbox);
        cylinders[c].sign = cstPrimitives[i].sign;
        cylinders[c].shapeID = cstPrimitives[i].shape;
        c++;
      }
      else if (cstPrimitives[i].type == "BOX") {
        boxes[b].set(cstPrimitives[i].bbox);
        boxes[b].sign = cstPrimitives[i].sign;
        boxes[b].shapeID = cstPrimitives[i].shape;
        b++;
      }
      else if (cstPrimitives[i].type == "DIRECTED_BOX") {
        directedBoxes[d].set(cstPrimitives[i].bbox);
        directedBoxes[d].width = cstPrimitives[i].meta[0];
        directedBoxes[d].height = cstPrimitives[i].meta[1];
        directedBoxes[d].shapeID = cstPrimitives[i].shape;
        d++;
      }
      else if (cstPrimitives[i].type == "POLYGON") {
        AABB bbox;

        if (cstPrimitives[i].sign < 0) {
          polygons[p].start_idx = static_cast<int>(v_polygon);
          polygons[p].size = -static_cast<int>(cstPrimitives[i].vertices.size());
        }
        else {
          polygons[p].start_idx = static_cast<int>(v_polygon);
          polygons[p].size = +static_cast<int>(cstPrimitives[i].vertices.size());
        }
        i_polygon += 2;

        float h = 50;

        if (cstPrimitives[i].sign < 0) {

          bbox.min = bbox.max = glm::vec3(cstPrimitives[i].vertices[0].x, 50/*i_polygon * 10*/, cstPrimitives[i].vertices[0].y);
          for (size_t t = 0; t < cstPrimitives[i].vertices.size(); t++) {
            glm::vec3 v(cstPrimitives[i].vertices[t].x, 50/*i_polygon * 10*/, cstPrimitives[i].vertices[t].y);
            polygonPoints[v_polygon++] = v;
            bbox.min = glm::min(bbox.min, v);
            bbox.max = glm::max(bbox.max, v);
          }
        }
        else
        {
          bbox.min = bbox.max = glm::vec3(cstPrimitives[i].vertices[0].x, h, cstPrimitives[i].vertices[0].y);
          for (size_t t = 0; t < cstPrimitives[i].vertices.size(); t++) {
            glm::vec3 v(cstPrimitives[i].vertices[t].x, h, cstPrimitives[i].vertices[t].y);
            polygonPoints[v_polygon++] = v;
            bbox.min = glm::min(bbox.min, v);
            bbox.max = glm::max(bbox.max, v);
          }
        }


        bbox.min.y = 0;
        /*min -= make_float3(scene_epsilon);
        max += make_float3(scene_epsilon);*/

        polygons[p].sign = cstPrimitives[i].sign;
        polygons[p].shapeID = cstPrimitives[i].shape;
        polygons[p].bbox = bbox;
        p++;
      }

      // TODO ShapeColors buggy!
      //shapeColors[cstPrimitives[i].shape] = cstPrimitives[i].color;
    }

}

void Application::create() {
  compileShaders();

  loadCST();

  for (Cylinder& c : cylinders) {
    glCylinders.addCylinder(c.center(), c.radius(), c.height(), c.sign);
  }
  glCylinders.createGLBuffer();
  glCylinders.gl_shader_program = 
  
//#define CYLINDER_AS_POINTS
#define CYLINDER_AS_BOXES
//#define CYLINDER_TESSELATED
#if defined(CYLINDER_AS_POINTS)
  compile_link_vs_fs(
    "../../src/glsl/cylinder/cylinder.vert",
    "../../src/glsl/cylinder/cylinder.frag"
  );
#elif defined(CYLINDER_AS_BOXES)
  compile_link_vs_gs_fs(
    "../../src/glsl/cylinder/cylinder.vert", 
    "../../src/glsl/cylinder/cylinder.geom", 
    "../../src/glsl/cylinder/cylinder.frag"
  );
#elif defined(CYLINDER_TESSELATED)
#endif

  glPolygons.setPoints(polygonPoints);
  for (PolygonShape& p : polygons) {
    glPolygons.addDrawElementsIndirectCommand(p.start_idx, p.size);
  }
  glPolygons.createGLBuffer();
  glPolygons.gl_shader_program =
    compile_link_vs_fs(
      "../../src/glsl/polygon/polygon.vert",
      "../../src/glsl/polygon/polygon.frag"
    );

  for (Box& b : boxes) {
    glBoxes.addBox(b.center(), b.dims());
  }
  glBoxes.createGLBuffer();
  glBoxes.gl_shader_program = 
    compile_link_vs_gs_fs(
      "../../src/glsl/box/box.vert",
      "../../src/glsl/box/box.geom",
      "../../src/glsl/box/box.frag"
    );

  for (DirectedBox& b : directedBoxes) {
    glDirectedBoxes.addDirectedBox(b.start, b.end, b.width, b.height);
  }
  /*float testVal = 1000.0f;
  glDirectedBoxes.addDirectedBox(glm::vec3(-testVal, -testVal, 0.0f), glm::vec3(testVal, testVal, 0.0f), 100.0f, 1000.0f);*/
  glDirectedBoxes.createGLBuffer();
  glDirectedBoxes.gl_shader_program =
    compile_link_vs_gs_fs(
      "../../src/glsl/directedbox/directedbox.vert",
      "../../src/glsl/directedbox/directedbox.geom",
      "../../src/glsl/directedbox/directedbox.frag"
    );
  
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

    mvp   = m_projmat * m_viewmat * m_worldmat;
    mv = m_viewmat * m_worldmat;

}

void Application::draw() {

  glViewport(0, 0, m_width, m_height);

  glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);
  glEnable(GL_DEPTH_TEST);
    
  float back_color[] = { 1, 1, 1, 1 };
  float zero[] = { 1.0f, 1.0f, 1.0f, 0.0f };
  float one = 1.0f;

  glClearBufferfv(GL_COLOR, 0, back_color);
  glClearBufferfv(GL_COLOR, 1, zero);
  glClearBufferfv(GL_DEPTH, 0, &one);
  glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  draw_CSG();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  GLint rendering_state_loc = glGetUniformLocation(ssao_program, "rendering_state");
  glUniform1i(rendering_state_loc, rendering_state);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_textures[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_textures[1]);
  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(quad_vao);

#ifdef TEST_PRIMITVES_DRAW
  glEnable(GL_DEPTH_TEST);
  glPolygons.draw(mvp);
  glCylinders.draw(mvp);
  glBoxes.draw(mvp);
  glDirectedBoxes.draw(mvp);
#endif
  

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Draw the world coordinate system
  glViewport(0, 0, 100, 100);
  glUseProgram(m_coord_system_program);
  glUniformMatrix4fv(0, 1, GL_FALSE, (GLfloat*)&mv);
  glDrawArrays(GL_LINES, 0, 6);
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
  m_coord_system_program  = compile_link_vs_fs("../../src/glsl/coord_sys.vert",           "../../src/glsl/coord_sys.frag");
  collect_fragments       = compile_link_vs_fs("../../src/glsl/fragmentCollecting.vert",  "../../src/glsl/fragmentCollecting.frag");
  resolve_csg_operations	= compile_link_vs_fs("../../src/glsl/quad.vert",                "../../src/glsl/csg.frag");
}

void Application::prepare_framebuffer() {
  glGenFramebuffers(1, &render_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);
  glGenTextures(3, fbo_textures);

  glBindTexture(GL_TEXTURE_2D, fbo_textures[0]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, fbo_textures[1]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, fbo_textures[2]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo_textures[0], 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, fbo_textures[1], 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fbo_textures[2], 0);

  static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

  glDrawBuffers(2, draw_buffers);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenVertexArrays(1, &quad_vao);
  glBindVertexArray(quad_vao);
}

void Application::prepare_fragment_collecting() {
  GLuint* data;

  // Create head pointer texture
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &head_pointer_texture);
  glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  glBindImageTexture(0, head_pointer_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);
  
  // Create buffer for clearing the head pointer texture
  glGenBuffers(1, &head_pointer_clear_buffer);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint), NULL, GL_STATIC_DRAW);
  data = (GLuint *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  memset(data, 0x00, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint));
  glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  // Create the atomic counter buffer
  glGenBuffers(1, &atomic_counter_buffer);
  glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
  glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

  // Create the linked list storage buffer
  glGenBuffers(1, &linked_list_buffer);
  glBindBuffer(GL_TEXTURE_BUFFER, linked_list_buffer);
  glBufferData(GL_TEXTURE_BUFFER, FRAMEBUFFER_MULTIPLIER * MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
  glBindBuffer(GL_TEXTURE_BUFFER, 0);

  // Bind it to a texture (for use as a TBO)
  glGenTextures(1, &linked_list_texture);
  glBindTexture(GL_TEXTURE_BUFFER, linked_list_texture);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, linked_list_buffer);
  glBindTexture(GL_TEXTURE_BUFFER, 0);

  glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);
}

void Application::draw_CSG() {
  GLuint * data;

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // Reset atomic counter
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
  data = (GLuint *)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
  data[0] = 0;
  glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

  // Clear head-pointer image
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER,    head_pointer_clear_buffer);
  glBindTexture(GL_TEXTURE_2D,            head_pointer_texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D,          0);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER,  0);

  // Bind head-pointer image for read-write
  glBindImageTexture(0, head_pointer_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

  // Bind linked-list buffer for write
  glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);
 
  glUseProgram(collect_fragments);

  glUniform1f(0, transparency_value);
  glUniform1i(2, 0);

  // disable constant color
  glUniform1i(5, 0);
  
  // Bind head-pointer image for read-write
  glBindImageTexture(0, head_pointer_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

  // Bind linked-list buffer for write
  glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

  glBindVertexArray(quad_vao);
  glUseProgram(resolve_csg_operations);
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