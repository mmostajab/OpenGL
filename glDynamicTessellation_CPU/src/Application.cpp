#ifdef WIN32
	//#include <Windows.h>
#endif

#define ENABLE_CULLING

#include "application.h"

// STD
#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <chrono>

// OpenMP
#include <omp.h>


// GL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "SampleDataGenerator.h"
#include "CameraInfo.h"

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
bool                 Application::wireframe = false;

Application::Application() {
  // get underlying buffer
  //m_orig_cout_buf = //std::cout.rdbuf();

  // set null
  //std::cout.rdbuf(NULL);

  m_logFile.open("log.h");
}

void Application::init(const unsigned int& width, const unsigned int& height, HGLRC mainWindowContext) {

    m_width = width; m_height = height;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    m_window = glfwCreateWindow(width, height, "Simple GL App", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);

	HGLRC currWindowContext = glfwGetWGLContext(m_window);
	wglShareLists(mainWindowContext, currWindowContext);

    glfwSetKeyCallback(m_window, key_callback);
    glfwSetWindowSizeCallback(m_window, WindowSizeCB);
    glfwSetCursorPosCallback(m_window, EventMousePos);
    glfwSetScrollCallback(m_window, EventMouseWheel);
    glfwSetMouseButtonCallback(m_window, (GLFWmousebuttonfun)EventMouseButton);
    glfwSetKeyCallback(m_window, (GLFWkeyfun)EventKey);

    // - Directly redirect GLFW char events to AntTweakBar
    glfwSetCharCallback(m_window, (GLFWcharfun)EventChar);

    if (glewInit() != GLEW_OK){
        //std::cout << "cannot intialize the glew.\n";
        exit(EXIT_FAILURE);
    }

    init();
}

void Application::init(const unsigned int& width, const unsigned int& height) {

    m_width = width; m_height = height;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    m_window = glfwCreateWindow(width, height, "Simple GL App", NULL, NULL);
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
        //std::cout << "cannot intialize the glew.\n";
        exit(EXIT_FAILURE);
    }

    init();

    GLenum e = glGetError();
    //glEnable(GL_DEPTH);
    e = glGetError();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    e = glGetError();
}


void Application::init() {
    GLenum e = glGetError();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    e = glGetError();

    m_camera.SetMode(MODELVIEWER);
    //m_camera.SetMode(FREE);
    m_camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_camera.SetLookAt(glm::vec3(0.0f, 0.0f, -3.0f));
    m_camera.SetClipping(0.01f, 100.0f);
    m_camera.SetFOV(60);
    m_camera.SetViewport(0, 0, m_width, m_height);
    m_camera.camera_scale = 0.01f;

    prepare_framebuffer();

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

#define ARC_SEGMENT
#ifdef ARC_SEGMENT

//#define ONE_ARC_SEGMENT
//#ifdef ONE_ARC_SEGMENT

#if 0
  ArcSegment arcSegment;
  arcSegment.p1 = glm::vec3(-1.0f, -1.0f, 0);
  arcSegment.p2 = glm::vec3( 1.0f,  1.0f, 0);
  //arcSegment.alpha = glm::pi<double>() / 2.0;
  arcSegment.center = glm::vec3(1.0f, -1.0f, 0.0f);
  arcSegment.createBuffer();
  arcSegments.push_back(arcSegment);
#endif


#endif

//#define MULTI_ARC_SEGMENT
#ifdef  MULTI_ARC_SEGMENT
  for (size_t i = 0; i < 10; i++) {
	  for (size_t j = 0; j < 10; j++) {
		  ArcSegment arcSegment;
		  glm::vec3 center(2 * i, 2 * j, 0.0f);
		  arcSegment.p1 = glm::vec3(-static_cast<float>(rand() % 1000) / 2000.0f - 0.5f, 0, 0) + center;
		  arcSegment.p2 = glm::vec3( 0, static_cast<float>(rand() % 1000) / 2000.0f + 0.5f, 0) + center;
		  //arcSegment.alpha = glm::pi<double>() / 2.0;
		  arcSegment.center = center;// glm::vec3(0.0f);
		  arcSegment.createBuffer(5);
		  arcSegments.push_back(arcSegment);
	  }
  }
#endif


//  addLogo(glm::vec3(0.0f), 0.0f, 1.0f, arcSegments, arcTriangles, arcQuads);

#define ARC_TRIANGLE
#ifdef  ARC_TRIANGLE

#if 0

  ArcTriangle arcTriangle;
  arcTriangle.p1 = glm::vec3(-1.0f, -1.0f, 0);
  arcTriangle.p3 = glm::vec3(1.0f,  1.0f, 0);
  arcTriangle.p2 = glm::vec3(-1.0f, 1.0f, 0);
  //arcSegment.alpha = glm::pi<double>() / 2.0;
  arcTriangle.center = glm::vec3(1.0f, -1.0f, 0.0f);
  arcTriangle.createBuffer();
  arcTriangles.push_back(arcTriangle);

#endif

#endif

#define ARC_QUAD
#ifdef ARC_QUAD

#if 0
  ArcQuad arcQuad;
  arcQuad.halfArcQuad[0].center = glm::vec3(0.0f);
  arcQuad.halfArcQuad[0].p0     = glm::vec3( 1.0f,  1.0f, 0.0f);
  arcQuad.halfArcQuad[0].p1     = glm::vec3(-1.0f, -1.0f, 0.0f);
  
  arcQuad.halfArcQuad[1].center = glm::vec3(0.0f);
  arcQuad.halfArcQuad[1].p0 = glm::vec3(2.0f, 2.0f, 0.0f);
  arcQuad.halfArcQuad[1].p1 = glm::vec3(-2.0f, -2.0f, 0.0f);

  arcQuad.createBuffer();

  arcQuads.push_back(arcQuad);

#endif

#if 1
  int32_t w = 6;
  int32_t h = 6;

  std::ifstream config("config.txt");
  if (config) {
    config >> w >> h;
  }

  float cst_width = 4.0f;
  float cst_height = 1.2f;

  for (int32_t i = 0; i < h; i++) {
    for (int32_t j = 0; j < w; j++) {
      addLogo(glm::vec3(-2.0f, 0.0f, 0.0f) + glm::vec3(i * cst_width, j * cst_height, 0.0f), 0.0f, 1.0f, arcSegments, arcTriangles, arcQuads);
      addCST(glm::vec3(0.25f, 0.0f, 0.0f) + glm::vec3(i * cst_width, j * cst_height, 0.0f), 1.0f * glm::pi<float>() / 6.0f, 1.0f, arcSegments, arcTriangles, arcQuads);
      //addLogo(glm::vec3(2.0f, 0.0f, 0.0f) + glm::vec3(i * cst_width, j * cst_height, 0.0f), 0.0f, 1.0f, arcSegments, arcTriangles, arcQuads);
    }
  }
#endif

#if 0

  ArcSegment seg;
  seg.set(
    glm::vec3(-100.0f, 0.0f, 0.0f),
    glm::vec3(+100.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, -1000.0f, 0.0f),
    100);
  arcSegments.push_back(seg);

#endif
  

  //addC(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f * glm::pi<float>() / 6.0f, 1.0f, arcSegments, arcTriangles, arcQuads);
  //addS(glm::vec3( 0.0f, 0.0f, 0.0f), 1.0f * glm::pi<float>() / 6.0f, 1.0f, arcSegments, arcTriangles, arcQuads);
  /*addT(glm::vec3( 1.0f, 0.0f, 0.0f), 1.0f * glm::pi<float>() / 6.0f, 1.0f, arcSegments, arcTriangles, arcQuads);*/
  //addCST(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f * glm::pi<float>() / 6.0f, 1.0f, arcSegments, arcTriangles, arcQuads);
/*
  addCST(glm::vec3(0.0f,-1.0f, 0.0f), -1.0f * glm::pi<float>() / 6.0f, 1.0f, arcSegments, arcTriangles, arcQuads);
  
  addCST(glm::vec3(0.0f, 2.0f, 0.0f), 3.0f * glm::pi<float>() / 6.0f, 1.0f, arcSegments, arcTriangles, arcQuads);*/



  /*addT(glm::vec3(0.0f), 1.0f * glm::pi<float>() / 3.0f, 0.2f, arcSegments, arcTriangles, arcQuads);
  addT(glm::vec3(0.0f), 2.0f * glm::pi<float>() / 3.0f, 0.2f, arcSegments, arcTriangles, arcQuads);
  addT(glm::vec3(0.0f), 3.0f * glm::pi<float>() / 3.0f, 0.2f, arcSegments, arcTriangles, arcQuads);
  addT(glm::vec3(0.0f), 4.0f * glm::pi<float>() / 3.0f, 0.2f, arcSegments, arcTriangles, arcQuads);
  addT(glm::vec3(0.0f), 5.0f * glm::pi<float>() / 3.0f, 0.2f, arcSegments, arcTriangles, arcQuads);*/
  //addM(arcSegments, arcTriangles, arcQuads);
  //addF(arcSegments, arcTriangles, arcQuads);

#endif

  //std::cout.set_rdbuf(m_orig_cout_buf);
  std::cout << "Number of Arc Segments = "  << arcSegments.size() << std::endl;
  std::cout << "Number of Arc Triangles = " << arcTriangles.size() << std::endl;
  std::cout << "Number of Arc Quads = "     << arcQuads.size() << std::endl;
  std::cout << "Total = " << arcSegments.size() + arcTriangles.size() + arcQuads.size() << std::endl;

  //std::cout.set_rdbuf(NULL);

  setupOpenGLSpecs();
}

void Application::update(float time, float timeSinceLastFrame) {

  if (m_w_pressed) {
    if (clock() - last_change_clock > 300) {
      wireframe = !wireframe;
      last_change_clock = clock();
    }
  }

  
    /*    m_camera.Move(CameraDirection::FORWARD);

    if (m_s_pressed)
        m_camera.Move(CameraDirection::BACK);

    if (m_a_pressed)
        m_camera.Move(CameraDirection::LEFT);

    if (m_d_pressed)
        m_camera.Move(CameraDirection::RIGHT);
        */
    if (m_q_pressed) {
      if (clock() - last_change_clock > 300) {
        for (auto& arc : arcSegments)  arc.multiplyTessFactor(2.0f);
        for (auto& arc : arcTriangles) arc.multiplyTessFactor(2.0f);
        for (auto& arc : arcQuads)     arc.multiplyTessFactor(2.0f);
        last_change_clock = clock();
      }
      //m_camera.Move(CameraDirection::UP);
    }

    if (m_e_pressed) {
      if (clock() - last_change_clock > 300) {
        for (auto& arc : arcSegments)  arc.multiplyTessFactor(0.5f);
        for (auto& arc : arcTriangles) arc.multiplyTessFactor(0.5f);
        for (auto& arc : arcQuads)     arc.multiplyTessFactor(0.5f);
        last_change_clock = clock();
      }
      //m_camera.Move(CameraDirection::DOWN);
    }

    // Updating the camera matrices
    m_camera.Update();
    m_camera.GetMatricies(m_projmat, m_viewmat, m_worldmat);
    m_inv_viewmat = glm::inverse(m_viewmat);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_transformation_buffer);
    glm::mat4* m_transformation_buffer = (glm::mat4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 3 * sizeof(glm::mat4), GL_MAP_WRITE_BIT);
    m_transformation_buffer[0] = m_projmat;
    m_transformation_buffer[1] = m_viewmat;
    m_transformation_buffer[2] = m_worldmat;
    glUnmapBuffer(GL_UNIFORM_BUFFER);

	clock_t start_time = clock();
	m_mvp_mat = m_projmat * m_viewmat * m_worldmat;

  float near_plane_width_w  = m_camera.camera_ortho_view_plane_size[0];
  float near_plane_height   = m_camera.camera_ortho_view_plane_size[1];
  
  // Passed camera information
  // TODO 
  // 1. should support rotated ortho cam
  // 2. should support perspective cam
  CameraInfo camInfo;
  camInfo.type = CameraInfo::CAMERA_TYPE_ORTHO;
  camInfo.ortho.near_plane_width_world  = m_camera.camera_ortho_view_plane_size[0];
  camInfo.ortho.near_plane_height_world = m_camera.camera_ortho_view_plane_size[1];
  camInfo.ortho.w = m_width;
  camInfo.ortho.h = m_height;


  std::chrono::high_resolution_clock::time_point start_update_time = std::chrono::high_resolution_clock::now();
//#pragma omp parallel
  {
//#pragma omp parallel for
    for (int i = 0; i < static_cast<int>(arcSegments.size()); i++)  arcSegments[i].updateBuffer(camInfo, m_mvp_mat, m_width, m_height);
//#pragma omp for
    for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++) arcTriangles[i].updateBuffer(camInfo, m_mvp_mat, m_width, m_height);
//#pragma omp for
    for (int i = 0; i < static_cast<int>(arcQuads.size()); i++)     arcQuads[i].updateBuffer(camInfo, m_mvp_mat, m_width, m_height);
  }

  int nTriangles = 0;
  {
    for (int i = 0; i < static_cast<int>(arcSegments.size()); i++)  nTriangles += arcSegments[i].getNumGenTriangles();
    for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++)  nTriangles += arcTriangles[i].getNumGenTriangles();
    for (int i = 0; i < static_cast<int>(arcQuads.size()); i++)  nTriangles += arcQuads[i].getNumGenTriangles();
  }
  std::cout << "Number of triangles = " << nTriangles << std::endl;

  std::chrono::high_resolution_clock::time_point end_update_time = std::chrono::high_resolution_clock::now();
  int64_t update_time = (end_update_time - start_update_time).count();

  std::cout << "performance (CPU Side) = " << static_cast<float>(nTriangles) / static_cast<float>(update_time / 1e9) << std::endl;

  if(update_time > 1e7) std::cout << "update time = " << update_time << "( " << 1e9f / update_time << " FPS )" <<std::endl;

	clock_t end_time = clock();
	////std::cout << "Tesselation time = " << (end_time - start_time) / CLOCKS_PER_SEC << std::endl;

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
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(2.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // background should be always filled.
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glUseProgram(m_background_program);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
  if (wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(m_simple_program);

  for (auto& arc : arcSegments)  arc.draw();
  for (auto& arc : arcTriangles) arc.draw();
  for (auto& arc : arcQuads)     arc.draw();

 

  // Draw the world coordinate system
  glViewport(0, 0, 100, 100);
  glUseProgram(m_coord_system_program);
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

void Application::run_onestep() {

  glfwMakeContextCurrent(m_window);

  double start_time;
  double start_frame;
  start_time = start_frame = glfwGetTime();

  if (!glfwWindowShouldClose(m_window))
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

	const char* coordsys_vert = {
		"#version 430 core\n"

		"layout(std140, binding = 0) uniform TransformWithTranslationBlock {\n"
			"mat4 proj_mat;\n"
			"mat4 view_mat;\n"
			"mat4 world_mat;\n"
		"} ;\n"

		"uniform vec3 position[6] = { vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f) };\n"
		"uniform vec3 color[3] = { vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f) };\n"

		"out vec4 vec_color;\n"

		"void main() {\n"
		"gl_Position =  vec4( mat3(view_mat * world_mat) * position[gl_VertexID], 1.0f);\n"
		"vec_color = vec4(color[gl_VertexID / 2], 1.0f);\n"
		"}\n"
	};

	const char* coordsys_frag = {
		"#version 430 core\n"

		"in vec4 vec_color;\n"

		"out vec4 out_color;\n"

		"void main() {\n"
			"out_color = vec_color;\n"
		"}\n"
	};

	const char* simple_vert = {
		"#version 430 core\n"

		"layout(std140, binding = 0) uniform TransformWithTranslationBlock {\n"
		"mat4 proj_mat;\n"
		"mat4 view_mat;\n"
		"mat4 world_mat;\n"
		"} ;\n"

		"layout ( location = 0 ) in vec3 position;\n"

		"void main(void)\n"
		"{\n"
		    "gl_Position = proj_mat * view_mat * world_mat * vec4(position, 1.0f);\n"
		"}\n"
	};

	const char* simple_frag = {
		"#version 430 core\n"

		// Final output
		"layout (location = 0) out vec4 color;\n"

		"void main(void) {\n"
			"color = vec4(0.4, 1, 0.6, 1);\n"
		"}\n"
	};

  const char* background_vert = {
    "#version 430 core\n"

    "out VS_OUT\n"
    "{\n"
      "vec4 E;\n"
    "} vs_out;\n"

    "void main(void)\n"
    "{\n"
    "  const vec4 vertices[] = vec4[](\n"
    "    vec4(-1.0f, -1.0f, 1.0f, 1.0f),\n"
    "    vec4( 1.0f, -1.0f, 1.0f, 1.0f),\n"
    "    vec4(-1.0f,  1.0f, 1.0f, 1.0f),\n"
    "    vec4( 1.0f,  1.0f, 1.0f, 1.0f) \n"
    ");\n"

    "  gl_Position = vertices[gl_VertexID];\n"
    "  vs_out.E = vertices[gl_VertexID];\n"
    "}\n"
  };

  const char* background_frag = {
    "#version 430 core\n"

    "in VS_OUT\n"
    "{\n"
    "vec4 E;\n"
    "} fs_in;\n"

    // Final output
    "layout (location = 0) out vec4 color;\n"

    "void main(void) {\n"
    "float t = (fs_in.E.y + 1.0f) / 2.0f;\n"
    "vec3 color1 = vec3(0.8f, 0.8f, 0.8f);\n"
    "vec3 color0 = vec3(0.3f, 0.3f, 0.3f);\n"
    "color = vec4((1.0f-t)*color0 + t*color1, 1);\n"
    "gl_FragDepth = 1.0f;\n"
    "}\n"
  };

  m_coord_system_program = compile_link_vs_fs_with_source(coordsys_vert, coordsys_frag);
  m_simple_program = compile_link_vs_fs_with_source(simple_vert, simple_frag);
  m_background_program = compile_link_vs_fs_with_source(background_vert, background_frag);
}

void Application::prepare_framebuffer() {
  glGenFramebuffers(1, &render_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);
  glGenTextures(3, fbo_textures);

  glBindTexture(GL_TEXTURE_2D, fbo_textures[0]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, 2048, 2048);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, fbo_textures[1]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 2048, 2048);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, fbo_textures[2]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 2048, 2048);
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

void Application::setupOpenGLSpecs()
{
  GLenum e = glGetError();
  //glEnable(GL_DEPTH);
  e = glGetError();
  glEnable(GL_DEPTH_TEST);
  e = glGetError();

#ifdef ENABLE_CULLING
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
#else
  glDisable(GL_CULL_FACE);
#endif
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