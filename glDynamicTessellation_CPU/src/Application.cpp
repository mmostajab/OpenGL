#ifdef WIN32
	//#include <Windows.h>
#endif

// #define FULL_SCREEN_ON_PRIMARY_MONITOR
//#define ENABLE_CULLING
//#define COMBINE_GL_BUFFERS

//#define MEASURE_RENDER_UPLOAD_TIME
//#define AUTO_CAM_MOVE_CASE2
#define TEST_CASE3

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
bool                  Application::wireframe = false;
AntTweakBarGUI        Application::m_gui;

Application::Application():
  trianglesPerSecondEvaluator("TrianglesPerSecond_GL"),
  memUsagePerTriangleEvaluator("MemPerTriangle_GL"),
  renderTimePerTriangleEvaluator("RenderTimePerTriangle_GL"),
  drawTimePerTriangleEvaluator("DrawTimePerTriangle_GL"),
  uploadTimePerTriangleEvaluator("UploadTimePerTriangleEvaluator_GL"),
  pipelinePerTrianglesEvaluator("PipelinePerTriangle_GL"),
  trianglesPerZoomFactorEvaluator("TrianglePerZoomFactor_GL"),
  red_color(1.0f, 0.0f, 0.0f),
  green_color(0.4f, 1.f, 0.6f)
{
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

    m_window = glfwCreateWindow(width, height, "Dynamic Arc Primitive Tessellator", NULL, NULL);
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


    m_window = glfwCreateWindow(width, height, "ArcPrimitve CPU Tessellator - Demo", 
#ifdef FULL_SCREEN_ON_PRIMARY_MONITOR
      glfwGetPrimaryMonitor(), 
#else
      NULL,
#endif
      NULL);

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
    
    m_gui.init(width, height);
}


void Application::init() {
    GLenum e = glGetError();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    e = glGetError();

    m_camera.SetMode(MODELVIEWER);
    //m_camera.SetMode(FREE);
#ifdef TEST_CASE3
    m_camera.SetPosition(glm::vec3(18.0f, 6.0f, 3.0f));
    m_camera.SetLookAt(glm::vec3(18.0f, 6.0f, -3.0f));
    m_camera.camera_ortho_view_plane_size[0] = 28.0;
    m_camera.camera_ortho_view_plane_size[1] = 28.0;
    m_camera.forward_scroll_down = 0.95f;
#else ifdef  TEST_CASE2
    m_camera.SetPosition(glm::vec3(0.0f, -0.75f, 6.0f));
    m_camera.SetLookAt(glm::vec3(0.0f, -0.75f, -6.0f));
    m_camera.camera_ortho_view_plane_size[0] = 11.0;
    m_camera.camera_ortho_view_plane_size[1] = 11.0;
    //m_camera.forward_scroll_up = 2.9f;
    //m_camera.forward_scroll_down = 0.7f;
#endif
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

    // GUI
    m_gui.general_wireframe                   = m_gui.general_wireframe_pre      =  false;
    m_gui.general_frustumCulling              = m_gui.general_frustumCulling_pre = true;//true;
    
    m_gui.general_increaseTriangles           = false;
    m_gui.general_decreaseTriangles           = false;
    m_gui.general_renderAABBs                 = false;
    m_gui.general_dropCullingFactor           = 1.0f;
    m_gui.general_triangulationAccuracyFactor = 1.0f;

    combineArcPrimitiveBuffers            = m_gui.general_combineBuffers   = m_gui.general_combineBuffers_pre   = true;
    updateEveryFrame                      = m_gui.general_updateEveryFrame = m_gui.general_updateEveryFrame_pre = false;

    for (size_t i = 0; i < NUMBER_OF_REQUIRED_COMBINATION_BUFFERS; i++) {
      allArcPrimitiveTrianglesBuffer[i]             = 0;
      allArcPrimitiveTrianglesIndirectDrawBuffer[i] = 0;
      allArcPrimitivesBufferSizeNumVertices[i]      = 0;
      allArcPrimitivesBufferSizeNumCmds[i]          = 0;
    }
    
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

#ifdef TEST_CASE5

  ArcSegment segment;
  segment.set(
    Vector3Df(-1.0f, 0.1f, 0),
    Vector3Df(-0.0f, 0.0f, 0),
    Vector3Df(-0.4f, 0.2f, 0),
    100
  );
  arcSegments.push_back(segment);

#endif

#ifdef TEST_CASE4

  ArcTriangle triangle0;
  triangle0.set(
    Vector3Df(-0.0f, 0.0f, 0),
    Vector3Df(-1.0f, 0.1f, 0),
    
    Vector3Df(-0.4f, 0.2f, 0),
    Vector3Df(-0.5f, 0.05f, 0),
    
    100
  );
  arcTriangles.push_back(triangle0);

#endif

#ifdef TEST_CASE1

  ArcSegment seg0, seg1;
  seg0.set(
    glm::vec3(-2.0f, 0.0f, 0.0f),
    glm::vec3(+2.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    100);
  arcSegments.push_back(seg0);

  seg1.set(
    glm::vec3(+2.0f, 0.0f, 0.0f),
    glm::vec3(-2.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    100);
  arcSegments.push_back(seg1);

#endif

#ifdef TEST_CASE2
  int nW = 80, nH = 80;
  float w = 10.0f, h = 10.0f;

  float r = std::min(w / nW, h / nH) / 2.0f;

  for (int i = 0; i < nH; i++)
    for (int j = 0; j < nW; j++) {

      ArcSegment seg0, seg1;

      float r_prime = r * (j+1) / nW;

      seg0.set(
        glm::vec3(-w / 2.0f, -h / 2.0f, 0.0f) + 2.0f * glm::vec3(j * r, i * r, 0.0f) - glm::vec3(r_prime, 0.0f, 0.0f),
        glm::vec3(-w / 2.0f, -h / 2.0f, 0.0f) + 2.0f * glm::vec3(j * r, i * r, 0.0f) + glm::vec3(r_prime, 0.0f, 0.0f),
        glm::vec3(-w / 2.0f, -h / 2.0f, 0.0f) + 2.0f * glm::vec3(j * r, i * r, 0.0f),
        100
      );

      seg1.set(
        glm::vec3(-w / 2.0f, -h / 2.0f, 0.0f) + 2.0f * glm::vec3(j * r, i * r, 0.0f) + glm::vec3(r_prime, 0.0f, 0.0f),
        glm::vec3(-w / 2.0f, -h / 2.0f, 0.0f) + 2.0f * glm::vec3(j * r, i * r, 0.0f) - glm::vec3(r_prime, 0.0f, 0.0f),
        glm::vec3(-w / 2.0f, -h / 2.0f, 0.0f) + 2.0f * glm::vec3(j * r, i * r, 0.0f), 
        100
      );

      arcSegments.push_back(seg0);
      arcSegments.push_back(seg1);

    }
#endif

#ifdef TEST_CASE3
  int32_t w = 200;
  int32_t h = 200;

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

  // Status setting
  m_gui.status_nVisible_primitives = static_cast<int>(arcQuads.size() + arcTriangles.size() + arcSegments.size());
  m_gui.status_nInvisible_primitives = 0;

  m_gui.general_combineBuffers = m_gui.general_combineBuffers_pre = true;
  disableSeparateGLBuffers();

  updateEveryFrame = m_gui.general_updateEveryFrame = m_gui.general_updateEveryFrame_pre = false;
  disableUpdateEveryFrame();

}

void Application::update(float time, float timeSinceLastFrame) {

  //glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);

#ifdef AUTO_CAM_MOVE_CASE2
  static int i = 0;
  static int nFrames = 0;
  const int maxSteps = 120;
  const int moveEveryNFrame = 40;

  if(i < maxSteps * moveEveryNFrame){
    nFrames++;
    if (nFrames > moveEveryNFrame) {
      m_camera.MoveForward(1);
      nFrames = 0;
      //m_camera.forward_scroll_down *= (1.0f + 1.0f / (2000.0f*i));
    }
    
    i++;
  }
  else {
    if (i == maxSteps * moveEveryNFrame)
      std::cout << "Test completed.\n";
    i++;
  }
#endif

  bool trianglesOfSingleBufferNeedUpdate = false;
  if (m_gui.general_combineBuffers != m_gui.general_combineBuffers_pre) {
    combineArcPrimitiveBuffers = m_gui.general_combineBuffers;
    m_gui.general_combineBuffers_pre = m_gui.general_combineBuffers;

    if (combineArcPrimitiveBuffers) {
      disableSeparateGLBuffers();
    }
    else {
      enableSeparateGLBuffers();
    }
  }

  if (m_gui.general_updateEveryFrame != m_gui.general_updateEveryFrame_pre) {
    updateEveryFrame = m_gui.general_updateEveryFrame;
    m_gui.general_updateEveryFrame_pre = m_gui.general_updateEveryFrame;

    if (updateEveryFrame) {
      enableUpdateEveryFrame();
    }
    else {
      disableUpdateEveryFrame();
    }
  }

  if (m_gui.general_frustumCulling_pre != m_gui.general_frustumCulling && !m_gui.general_frustumCulling) {
    for (int i = 0; i < static_cast<int>(arcSegments.size()); i++) {
      arcSegments[i].enable();
    }

    for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++) {
      arcTriangles[i].enable();
    }

    for (int i = 0; i < static_cast<int>(arcQuads.size()); i++) {
      arcQuads[i].enable();
    }

    trianglesOfSingleBufferNeedUpdate = true;
  }
  m_gui.general_frustumCulling_pre = m_gui.general_frustumCulling;

  if (m_w_pressed || m_gui.general_wireframe != m_gui.general_wireframe_pre) {
    if (clock() - last_change_clock > 300) {
      wireframe = m_gui.general_wireframe;
      last_change_clock = clock();
      m_gui.general_wireframe_pre = m_gui.general_wireframe;
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
    if (m_q_pressed || m_gui.general_decreaseTriangles) {
      if (clock() - last_change_clock > 300) {
        for (auto& arc : arcSegments)  arc.multiplyTessFactor(2.0f);
        for (auto& arc : arcTriangles) arc.multiplyTessFactor(2.0f);
        for (auto& arc : arcQuads)     arc.multiplyTessFactor(2.0f);
        last_change_clock = clock();
      }
      m_gui.general_decreaseTriangles = false;
      //m_camera.Move(CameraDirection::UP);
      trianglesOfSingleBufferNeedUpdate = true;
    }

    if (m_e_pressed || m_gui.general_increaseTriangles) {
      if (clock() - last_change_clock > 300) {
        for (auto& arc : arcSegments)  arc.multiplyTessFactor(0.5f);
        for (auto& arc : arcTriangles) arc.multiplyTessFactor(0.5f);
        for (auto& arc : arcQuads)     arc.multiplyTessFactor(0.5f);
        last_change_clock = clock();
      }
      m_gui.general_increaseTriangles = false;
      //m_camera.Move(CameraDirection::DOWN);
      trianglesOfSingleBufferNeedUpdate = true;
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

  // Apply GUI
  float dropCullingFactor = m_gui.general_dropCullingFactor * m_gui.general_dropCullingFactor;
  for (int i = 0; i < static_cast<int>(arcSegments.size()); i++) {
    arcSegments[i].setDropCullingFactor(dropCullingFactor);
    arcSegments[i].setTrianulationAccuracyFactor(m_gui.general_triangulationAccuracyFactor);
  }
  for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++) {
    arcTriangles[i].setDropCullingFactor(dropCullingFactor);
    arcTriangles[i].setTrianulationAccuracyFactor(m_gui.general_triangulationAccuracyFactor);
  }

  for (int i = 0; i < static_cast<int>(arcQuads.size()); i++) {
    arcQuads[i].setDropCullingFactor(dropCullingFactor);
    arcQuads[i].setTrianulationAccuracyFactor(m_gui.general_triangulationAccuracyFactor);
  }



  //float near_plane_width_w  = m_camera.camera_ortho_view_plane_size[0];
  //float near_plane_height   = m_camera.camera_ortho_view_plane_size[1];

  glm::mat4 invProjMatrix = glm::inverse(m_projmat);
  glm::vec4 c[8];
  
  c[0] = invProjMatrix * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
  c[1] = invProjMatrix * glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f);
  c[2] = invProjMatrix * glm::vec4( 1.0f,  1.0f, -1.0f, 1.0f);
  c[3] = invProjMatrix * glm::vec4( 1.0f, -1.0f, -1.0f, 1.0f);
  c[4] = invProjMatrix * glm::vec4(-1.0f, -1.0f,  1.0f, 1.0f);
  c[5] = invProjMatrix * glm::vec4(-1.0f,  1.0f,  1.0f, 1.0f);
  c[6] = invProjMatrix * glm::vec4( 1.0f,  1.0f,  1.0f, 1.0f);
  c[7] = invProjMatrix * glm::vec4( 1.0f, -1.0f,  1.0f, 1.0f);


  float near_plane_width_world = fabs(c[2][0] - c[1][0]);
  float near_plane_height_world = fabs(c[0][1] - c[1][1]);
  
  // Passed camera information
  // TODO 
  // 1. should support rotated ortho cam
  // 2. should support perspective cam
  CameraInfo camInfo;
  camInfo.type = CameraInfo::CAMERA_TYPE_ORTHO;
  camInfo.ortho.near_plane_width_world = near_plane_width_world; //m_camera.camera_ortho_view_plane_size[0];
  camInfo.ortho.near_plane_height_world = near_plane_height_world; //m_camera.camera_ortho_view_plane_size[1];
  camInfo.ortho.w = m_width;
  camInfo.ortho.h = m_height;

  //std::cout << " Pixel width = " << camInfo.ortho.pixelWidth() << " Pixel height = " << camInfo.ortho.pixelHeight() << std::endl;

//#define USE_WINDOWS_TIME
#ifdef USE_WINDOWS_TIME
  DWORD start_update_time = timeGetTime();
#else
  std::chrono::high_resolution_clock::time_point start_update_time = std::chrono::high_resolution_clock::now();
#endif

  // ================================
  // == Frustum Culling
  // ================================
  m_gui.status_nVisible_primitives = 0;
  m_gui.status_nInvisible_primitives = 0;
  if(m_gui.general_frustumCulling)
  {
    std::chrono::high_resolution_clock::time_point start_frustum_culling_time = std::chrono::high_resolution_clock::now();
    Frustum frustum = UnifiedMath::getFrustum(m_mvp_mat);

#pragma omp parallel
    {
#pragma omp for
      for (int i = 0; i < static_cast<int>(arcSegments.size()); i++) {
        if (UnifiedMath::FrustumAABBIntersect(frustum, arcSegments[i].getAABB())) {
          arcSegments[i].enable();
          //m_gui.status_nVisible_primitives++;
        }
        else {
          arcSegments[i].disable();
          //m_gui.status_nInvisible_primitives++;
        }
      }

#pragma omp for
      for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++) {
        if (UnifiedMath::FrustumAABBIntersect(frustum, arcTriangles[i].getAABB())) {
          arcTriangles[i].enable();
          //m_gui.status_nVisible_primitives++;
        }
        else {
          arcTriangles[i].disable();
          //m_gui.status_nInvisible_primitives++;
        }
      }

#pragma omp for
      for (int i = 0; i < static_cast<int>(arcQuads.size()); i++) {
        if (UnifiedMath::FrustumAABBIntersect(frustum, arcQuads[i].getAABB())) {
          arcQuads[i].enable();
          //m_gui.status_nVisible_primitives++;
        }
        else {
          arcQuads[i].disable();
          //m_gui.status_nInvisible_primitives++;
        }
      }

#pragma omp barrier 
    }
    std::chrono::high_resolution_clock::time_point end_frustum_culling_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> frustum_culling_time_span = std::chrono::duration_cast<std::chrono::duration<float>>(end_frustum_culling_time - start_frustum_culling_time);
    m_gui.status_frustum_culling_time = frustum_culling_time_span.count() * 1000.0f;
  }
  else {
    //m_gui.status_nVisible_primitives = static_cast<int>(arcQuads.size() + arcTriangles.size() + arcSegments.size());
    //m_gui.status_nInvisible_primitives = 0;
    m_gui.status_frustum_culling_time = 0;
  }

  // if the frustum culling is deactivated, all the primitives should change their state to activated.
  if (m_gui.general_frustumCulling_pre != m_gui.general_frustumCulling && !m_gui.general_frustumCulling) {
    for (int i = 0; i < static_cast<int>(arcSegments.size()); i++) {
      arcSegments[i].enable();
    }

    for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++) {
      arcTriangles[i].enable();
    }

    for (int i = 0; i < static_cast<int>(arcQuads.size()); i++) {
      arcQuads[i].enable();
    }
  }
  m_gui.general_frustumCulling_pre = m_gui.general_frustumCulling;

  // ==================================
  // == Update Arc Primitives
  // ==================================

  omp_set_num_threads(omp_get_max_threads());

  std::chrono::high_resolution_clock::time_point start_triangulation_time = std::chrono::high_resolution_clock::now();
#pragma omp parallel
  {
#pragma omp for
    for (int i = 0; i < static_cast<int>(arcSegments.size()); i++) {
        if(arcSegments[i].updateBuffer(camInfo, m_mvp_mat, m_width, m_height))
          trianglesOfSingleBufferNeedUpdate = true;
    }
#pragma omp for
    for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++) {
        if(arcTriangles[i].updateBuffer(camInfo, m_mvp_mat, m_width, m_height))
          trianglesOfSingleBufferNeedUpdate = true;
    }
#pragma omp for
    for (int i = 0; i < static_cast<int>(arcQuads.size()); i++) {
        if(arcQuads[i].updateBuffer(camInfo, m_mvp_mat, m_width, m_height))
          trianglesOfSingleBufferNeedUpdate = true;
    }

#pragma omp barrier 
  }

  std::chrono::high_resolution_clock::time_point end_triangulation_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> triangulation_time_span = std::chrono::duration_cast<std::chrono::duration<float>>(end_triangulation_time - start_triangulation_time);
  m_gui.status_triangulation_time = triangulation_time_span.count() * 1000.0f;

  // ===========================================
  // == Buffer Combination
  // ===========================================

  float buffer_combination_time = 0;
  if (combineArcPrimitiveBuffers && trianglesOfSingleBufferNeedUpdate) {
    //std::cout << "update.\n";
    std::chrono::high_resolution_clock::time_point start_buffer_combination_time = std::chrono::high_resolution_clock::now();
    updateAllArcPrimitivesSingleBuffer();
    std::chrono::high_resolution_clock::time_point end_buffer_combination_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> buffer_combination_time_span = std::chrono::duration_cast<std::chrono::duration<float>>(end_buffer_combination_time - start_buffer_combination_time);
    buffer_combination_time = buffer_combination_time_span.count() * 1000.0f;

    std::cout << "Buffer combination time = " << buffer_combination_time << " ms" << std::endl;
  }
  else {
    buffer_combination_time = 0;
  }

  // ===========================================
  // == whole update time
  // ===========================================

#ifdef USE_WINDOWS_TIME
  DWORD end_update_time = timeGetTime();
  DWORD update_time = (end_update_time - start_update_time);
  m_gui.status_update_time = static_cast<float>(update_time);
  m_gui.status_perf_triangles_per_second = static_cast<float>(nTriangles) / static_cast<float>(update_time / 1e3);
#else
  std::chrono::high_resolution_clock::time_point end_update_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(end_update_time - start_update_time);
  float update_time = time_span.count() * 1000.0f;
  m_gui.status_update_time = update_time;
  m_gui.status_perf_triangles_per_second = static_cast<float>(nTriangles) / static_cast<float>(update_time);
#endif

  // ===========================================
  // == Status of primitives and performance
  // ===========================================
  nTriangles = 0;
  memUsage  = 0.f;
  {
    for (int i = 0; i < static_cast<int>(arcSegments.size()); i++) {
      int nGenTriangles = arcSegments[i].getNumGenTriangles();
      nTriangles += nGenTriangles;
      memUsage += arcSegments[i].getFilledMemBytes();

      if (nGenTriangles <= 0) {
        m_gui.status_nInvisible_primitives++;
      }
      else {
        m_gui.status_nVisible_primitives++;
      }
    }
    for (int i = 0; i < static_cast<int>(arcTriangles.size()); i++) {
      int nGenTriangles = arcTriangles[i].getNumGenTriangles();
      nTriangles += nGenTriangles;
      memUsage += arcTriangles[i].getFilledMemBytes();

      if (nGenTriangles <= 0) {
        m_gui.status_nInvisible_primitives++;
      }
      else {
        m_gui.status_nVisible_primitives++;
      }
    }
    for (int i = 0; i < static_cast<int>(arcQuads.size()); i++) {
      int nGenTriangles = arcQuads[i].getNumGenTriangles();
      nTriangles += nGenTriangles;
      memUsage += arcQuads[i].getFilledMemBytes();

      if (nGenTriangles <= 0) {
        m_gui.status_nInvisible_primitives++;
      }
      else {
        m_gui.status_nVisible_primitives++;
      }
    }
  }
  
  //std::cout << "Number of triangles = " << nTriangles << std::endl;
  m_gui.status_nTriangles = nTriangles;

  m_gui.status_buffer_combination_time = buffer_combination_time;

  //std::cout << "performance (CPU Side) = " << static_cast<float>(nTriangles) / static_cast<float>(update_time / 1e9) << std::endl;
  //if (update_time > 1e7) std::cout << "update time = " << update_time << "( " << 1e9f / update_time << " FPS )" << std::endl;
  

  // ==================================================
  // == Evaluation part
  // ==================================================
  
  memUsage /= (1024.0f * 1024.0f);
  trianglesPerSecondEvaluator.addData(nTriangles, m_gui.status_update_time);
  memUsagePerTriangleEvaluator.addData(nTriangles, memUsage);
  
  if (m_gui.general_writeEvaluations) {
    std::string path = "c:/CST/Latest";
    trianglesPerSecondEvaluator.output(path);
    memUsagePerTriangleEvaluator.output(path);
    renderTimePerTriangleEvaluator.output(path);
    drawTimePerTriangleEvaluator.output(path);
    uploadTimePerTriangleEvaluator.output(path);
    pipelinePerTrianglesEvaluator.output(path);
    trianglesPerZoomFactorEvaluator.output(path);
    m_gui.general_writeEvaluations = false;
  }

  if (m_gui.general_clearEvaluations) {
    trianglesPerSecondEvaluator.clearData();
    memUsagePerTriangleEvaluator.clearData();
    renderTimePerTriangleEvaluator.clearData();
    drawTimePerTriangleEvaluator.clearData();
    uploadTimePerTriangleEvaluator.clearData();
    pipelinePerTrianglesEvaluator.clearData();
    trianglesPerZoomFactorEvaluator.clearData();
    m_gui.general_clearEvaluations = false;
  }

	clock_t end_time = clock();
	////std::cout << "Tesselation time = " << (end_time - start_time) / CLOCKS_PER_SEC << std::endl;

  // ====================================================
  // == updating the lighting info
  // ====================================================
  //glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_lighting_buffer);
  //glm::vec4* light_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
  //light_info[0] = glm::vec4(-1, -1, -1, 0);
  //light_info[1] = glm::vec4(m_camera.getPosition(), 1.0f);
  //glUnmapBuffer(GL_UNIFORM_BUFFER);

  // =====================================================
  // == updating the general information for every object
  // =====================================================
  //glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_general_buffer);
  //glm::vec4* general_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
  //glUnmapBuffer(GL_UNIFORM_BUFFER);
}

#define USE_BOOST_TIMER_PERFORMANCE

void Application::draw(bool doUpdateGLBuffers) {
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

  static int trigger = 0;

  static bool  firstRun = true;
  static float updateDrawTime, drawTime;

#ifdef MEASURE_RENDER_UPLOAD_TIME
  glFinish();
#endif

#ifdef USE_BOOST_TIMER_PERFORMANCE
  std::chrono::high_resolution_clock::time_point start_render_time = std::chrono::high_resolution_clock::now();
#else
  double start_render_time = glfwGetTime();
#endif

  if (combineArcPrimitiveBuffers){
    glUseProgram(m_simple_program);
    glUniform3fv(0, 1, (GLfloat*)&green_color);
  
    glEnableVertexAttribArray(0);

    {
      size_t i = TRIANGLE_FAN_BUFFER_ID;
      
      glBindBuffer(GL_ARRAY_BUFFER, allArcPrimitiveTrianglesBuffer[i]);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

      glBindBuffer(GL_DRAW_INDIRECT_BUFFER, allArcPrimitiveTrianglesIndirectDrawBuffer[i]);
      glMultiDrawArraysIndirect(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(indirectDrawCmds[i].size()), 0);
    }

    {
      size_t i = TRIANGLE_STRIP_BUFFER_ID;

      glBindBuffer(GL_ARRAY_BUFFER, allArcPrimitiveTrianglesBuffer[i]);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

      glBindBuffer(GL_DRAW_INDIRECT_BUFFER, allArcPrimitiveTrianglesIndirectDrawBuffer[i]);
      glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(indirectDrawCmds[i].size()), 0);
    }

    glDisableVertexAttribArray(0);
  } else {

    glUseProgram(m_simple_program);
    glUniform3fv(0, 1, (GLfloat*)&green_color);
    for (auto& arc : arcSegments)  arc.draw(doUpdateGLBuffers);
    for (auto& arc : arcTriangles) arc.draw(doUpdateGLBuffers);
    for (auto& arc : arcQuads)     arc.draw(doUpdateGLBuffers);

  }

#ifdef MEASURE_RENDER_UPLOAD_TIME
  glFinish();
#endif

  //double end_render_time = glfwGetTime();

#ifdef USE_BOOST_TIMER_PERFORMANCE
  std::chrono::high_resolution_clock::time_point end_render_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(end_render_time - start_render_time);
#else
  glGetQueryiv(GL_ARB_timer_query, )
  float time_span = static_cast<float>(end_render_time - start_render_time);
#endif  

  if (doUpdateGLBuffers) {

#ifdef USE_BOOST_TIMER_PERFORMANCE
    updateDrawTime = time_span.count() * 1000.0f;
#else
    updateDrawTime = time_span * 1000.0f;
#endif

  }
  else {

#ifdef USE_BOOST_TIMER_PERFORMANCE
    drawTime = time_span.count() * 1000.0f;
#else
    drawTime = time_span * 1000.0f;
#endif

  }

  if (!firstRun) {
    if (!doUpdateGLBuffers) {
      drawTimePerTriangleEvaluator.addData(nTriangles, drawTime);
      uploadTimePerTriangleEvaluator.addData(nTriangles, updateDrawTime - drawTime);
      renderTimePerTriangleEvaluator.addData(nTriangles, updateDrawTime);
    }
  }
  else
    firstRun = false;
  

  if (m_gui.general_renderAABBs) {
    glUniform3fv(0, 1, (GLfloat*)&red_color);
    for (auto& arc : arcSegments)  arc.drawBoundingBox();
    for (auto& arc : arcTriangles) arc.drawBoundingBox();
    for (auto& arc : arcQuads)     arc.drawBoundingBox();
  }
 

  // Draw the world coordinate system
  glViewport(0, 0, 100, 100);
  glUseProgram(m_coord_system_program);
  glDrawArrays(GL_LINES, 0, 6);

  m_gui.draw();
}

void Application::run() {
  create();
  double start_time = glfwGetTime();
  double start_frame_time;

#ifdef MEASURE_RENDER_UPLOAD_TIME
  static int v = 0;
#endif

  while (!glfwWindowShouldClose(m_window))
  {
    start_frame_time = glfwGetTime();

#ifdef MEASURE_RENDER_UPLOAD_TIME
    bool ping = v++ % 2 == 0;
#else
    const bool ping = true;
#endif
    //bool disableUpdate = true;

#ifdef MEASURE_RENDER_UPLOAD_TIME
    if (ping) 
#endif
      update(0.0f, 0.0f);

    double frame_draw_start_time = glfwGetTime();
    draw(ping);
    glfwSwapBuffers(m_window);
    double frame_draw_end_time = glfwGetTime();

    glfwPollEvents();

    double end_frame_time = glfwGetTime();

    double current_time = glfwGetTime();
    double elapsed_since_start          = current_time - start_frame_time;
    double elapsed_since_last_frame     = current_time - start_frame_time;
    double draw_time                    = frame_draw_end_time - frame_draw_start_time;
    double frame_time                   = end_frame_time - start_frame_time;

    m_gui.status_draw_time = static_cast<float>(draw_time * 1000);
    m_gui.status_overall_fps = static_cast<float>(1.0 / frame_time);
    
    //update(static_cast<float>(elapsed_since_start), static_cast<float>(elapsed_since_last_frame));

    current_time = glfwGetTime();

    //if (elapsed_since_start > 5) {
      trianglesPerZoomFactorEvaluator.addData(static_cast<float>(1.f / m_camera.camera_ortho_view_plane_size[0]), nTriangles);
      pipelinePerTrianglesEvaluator.addData(static_cast<float>(nTriangles), static_cast<float>(current_time - start_frame_time)*1000.0f);
    //}
  }
}

void Application::run_onestep() {

  static int v = 1;
  glfwMakeContextCurrent(m_window);

  double start_time;
  double start_frame;
  start_time = start_frame = glfwGetTime();

  if (!glfwWindowShouldClose(m_window))
  {
    bool disableUpdate = v++ % 2 == 1;

    double frame_start_time = glfwGetTime();
    draw(disableUpdate);
    double frame_end_time = glfwGetTime();

    glfwSwapBuffers(m_window);
    glfwPollEvents();

    double current_time = glfwGetTime();
    double elapsed_since_start          = current_time - start_time;
    double elapsed_since_last_frame     = current_time - start_frame;

    start_frame = glfwGetTime();

    if(!disableUpdate) update(static_cast<float>(elapsed_since_start), static_cast<float>(elapsed_since_last_frame));
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

    "layout (location = 0) uniform vec3 renderColor;\n"

		// Final output
		"layout (location = 0) out vec4 color;\n"

		"void main(void) {\n"
			"color = vec4(renderColor, 1);\n"
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

  m_gui.TwEventMouseButtonGLFW3(button, action, mods);
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

  m_gui.TwEventMousePosGLFW3(xpos, ypos);
}

void Application::EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {
  m_camera.MoveForward(static_cast<int>(yoffset));

  m_gui.TwEventMouseWheelGLFW3(xoffset, yoffset);
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

  m_gui.TwEventKeyGLFW3(key, scancode, action, mods);
}

void Application::EventChar(GLFWwindow* window, int codepoint) {
  m_gui.TwEventCharGLFW3(codepoint);
}

// Callback function called by GLFW when window size changes
void Application::WindowSizeCB(GLFWwindow* window, int width, int height) {
  m_width = width; m_height = height;
  glViewport(0, 0, width, height);
  m_gui.WindowSizeCB(width, height);
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



void Application::enableUpdateEveryFrame()
{
  for (auto& arc : arcSegments)  arc.enableUpdateEveryFrame();
  for (auto& arc : arcTriangles) arc.enableUpdateEveryFrame();
  for (auto& arc : arcQuads)     arc.enableUpdateEveryFrame();
}

void Application::disableUpdateEveryFrame()
{
  for (auto& arc : arcSegments)  arc.disableUpdateEveryFrame();
  for (auto& arc : arcTriangles) arc.disableUpdateEveryFrame();
  for (auto& arc : arcQuads)     arc.disableUpdateEveryFrame();
}

void Application::disableSeparateGLBuffers()
{
  for (auto& arc : arcSegments)  arc.disableGLBufferCreation();
  for (auto& arc : arcTriangles) arc.disableGLBufferCreation();
  for (auto& arc : arcQuads)     arc.disableGLBufferCreation();
}

void Application::enableSeparateGLBuffers()
{
  for (auto& arc : arcSegments)  arc.enableGLBufferCreation();
  for (auto& arc : arcTriangles) arc.enableGLBufferCreation();
  for (auto& arc : arcQuads)     arc.enableGLBufferCreation();
}

void Application::updateAllArcPrimitivesSingleBuffer()
{
  auto& triangleFanVertices   = primitiveVertices[TRIANGLE_FAN_BUFFER_ID];
  auto& triangleFanCmds       = indirectDrawCmds[TRIANGLE_FAN_BUFFER_ID];
  auto& triangleStripVertices = primitiveVertices[TRIANGLE_STRIP_BUFFER_ID];
  auto& triangleStripCmds     = indirectDrawCmds[TRIANGLE_STRIP_BUFFER_ID];

  triangleFanVertices.clear();
  triangleFanCmds.clear();
  triangleStripVertices.clear();
  triangleStripCmds.clear();

  for (auto& arc : arcSegments) {
    const std::vector<Vertex>& arcSegVertices = arc.getTriangleVertices();

    DrawArraysIndirectCommand drawCmd;
    drawCmd.instanceCount = 1;
    drawCmd.baseInstance = 0;
    drawCmd.first = static_cast<uint32_t>(triangleFanVertices.size());
    drawCmd.count = static_cast<uint32_t>(arcSegVertices.size());

    // At least there should a triangle inside
    if (arcSegVertices.size() > 2) {
      triangleFanVertices.insert(triangleFanVertices.end(), arcSegVertices.begin(), arcSegVertices.end());
      triangleFanCmds.push_back(drawCmd);
    }
 
  }

  for (auto& arc : arcTriangles) {
    const std::vector<Vertex>& arcTriangleVertices = arc.getTriangleVertices();

    DrawArraysIndirectCommand drawCmd;
    drawCmd.instanceCount = 1;
    drawCmd.baseInstance = 0;
    drawCmd.first = static_cast<uint32_t>(triangleFanVertices.size());
    drawCmd.count = static_cast<uint32_t>(arcTriangleVertices.size());

    if (arcTriangleVertices.size() > 2) {
      triangleFanVertices.insert(triangleFanVertices.end(), arcTriangleVertices.begin(), arcTriangleVertices.end());
      triangleFanCmds.push_back(drawCmd);
    }
  }

  for (auto& arc : arcQuads) {
    const std::vector<Vertex>& arcQuadVertices = arc.getTriangleVertices();

    DrawArraysIndirectCommand drawCmd;
    drawCmd.instanceCount = 1;
    drawCmd.baseInstance = 0;
    drawCmd.first = static_cast<uint32_t>(triangleStripVertices.size());
    drawCmd.count = static_cast<uint32_t>(arcQuadVertices.size());

    if (arcQuadVertices.size() > 2) {
      triangleStripVertices.insert(triangleStripVertices.end(), arcQuadVertices.begin(), arcQuadVertices.end());
      triangleStripCmds.push_back(drawCmd);
    }
  }

  for (size_t i = 0; i < NUMBER_OF_REQUIRED_COMBINATION_BUFFERS; i++) {
    if (allArcPrimitivesBufferSizeNumVertices[i] < primitiveVertices[i].size()) {
      if (allArcPrimitiveTrianglesBuffer[i] > 0)
        glDeleteBuffers(1, &allArcPrimitiveTrianglesBuffer[i]);
      
      glGenBuffers(1, &allArcPrimitiveTrianglesBuffer[i]);
      glBindBuffer(GL_ARRAY_BUFFER, allArcPrimitiveTrianglesBuffer[i]);
      glBufferData(GL_ARRAY_BUFFER, primitiveVertices[i].size() * sizeof(Vertex), primitiveVertices[i].data(), GL_STATIC_DRAW);
      allArcPrimitivesBufferSizeNumVertices[i] = primitiveVertices[i].size();
    }
    else {
      glBindBuffer(GL_ARRAY_BUFFER, allArcPrimitiveTrianglesBuffer[i]);
      glBufferSubData(GL_ARRAY_BUFFER, 0, primitiveVertices[i].size() * sizeof(Vertex), primitiveVertices[i].data());
    }

    if (allArcPrimitivesBufferSizeNumCmds[i] < indirectDrawCmds[i].size()) {
      if (allArcPrimitiveTrianglesIndirectDrawBuffer[i] > 0)
        glDeleteBuffers(1, &allArcPrimitiveTrianglesIndirectDrawBuffer[i]);

      glGenBuffers(1, &allArcPrimitiveTrianglesIndirectDrawBuffer[i]);
      glBindBuffer(GL_DRAW_INDIRECT_BUFFER, allArcPrimitiveTrianglesIndirectDrawBuffer[i]);
      glBufferData(GL_DRAW_INDIRECT_BUFFER, indirectDrawCmds[i].size() * sizeof(DrawArraysIndirectCommand), indirectDrawCmds[i].data(), GL_STATIC_DRAW);
      allArcPrimitivesBufferSizeNumCmds[i] = indirectDrawCmds[i].size();
    }
    else {
      glBindBuffer(GL_DRAW_INDIRECT_BUFFER, allArcPrimitiveTrianglesIndirectDrawBuffer[i]);
      glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, indirectDrawCmds[i].size() * sizeof(DrawArraysIndirectCommand), indirectDrawCmds[i].data());
    }
  }
}
