#ifdef WIN32
	#include <Windows.h>
#endif

#include "application.h"
#include "plydatareader.h"

// STD
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>

// GL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#define PORSCHE_MESH
//#define SPHERE_MESH

Application::Application() {
}

void Application::init(const unsigned int& width, const unsigned int& height) {

    m_width = width; m_height = height;

    glfwSetErrorCallback(::error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    m_window = glfwCreateWindow(width, height, "Navigation Test Application", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);

    glfwSetKeyCallback(m_window, ::key_callback);
    glfwSetWindowSizeCallback(m_window, ::WindowSizeCB);
    glfwSetCursorPosCallback(m_window, ::EventMousePos);
    glfwSetScrollCallback(m_window, ::EventMouseWheel);
    glfwSetMouseButtonCallback(m_window, (GLFWmousebuttonfun)::EventMouseButton);
    glfwSetKeyCallback(m_window, (GLFWkeyfun)::EventKey);
    // - Directly redirect GLFW char events to AntTweakBar
    glfwSetCharCallback(m_window, (GLFWcharfun)::EventChar);

	// Sets this object as the passed data
	glfwSetWindowUserPointer(m_window, this);


    if (glewInit() != GLEW_OK){
        std::cout << "cannot intialize the glew.\n";
        exit(EXIT_FAILURE);
    }

    init();

    GLenum e;
    glEnable(GL_DEPTH_TEST);
    e = glGetError();
}

void Application::init() {
    GLenum e = glGetError();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    e = glGetError();

	m_fov = glm::pi<double>() / 3.0;
	m_aspectRatio = static_cast<double>(m_width) / m_height;
	m_zNear = 0.01;
  m_arcBallRadius = 0.7;

	m_navigation.setProject(m_fov, m_aspectRatio, m_zNear);
	m_navigation.setView(glm::dvec3(1.0), glm::dvec3(0.0), glm::dvec3(0.0, 1.0, 0.0));
	m_navigation.setScreenSize(m_width, m_height);

    //m_camera.SetMode(MODELVIEWER);
    ////m_camera.SetMode(FREE);
    //m_camera.SetPosition(glm::vec3(3.0f, 3.0f, 3.0f));
    //m_camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    //m_camera.SetClipping(0.01f, 100.0f);
    //m_camera.SetFOV(60);
    //m_camera.SetViewport(0, 0, m_width, m_height);
    //m_camera.camera_scale = 0.01f;

    glGenBuffers(1, &m_transformation_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_transformation_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_lighting_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_lighting_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_general_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_general_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	back_color = glm::vec4(0.2f, 0.6f, 0.7f, 1.0f);
	one = 1.0f;

	// Static Members
	m_controlKeyHold = false;
	m_altKeyHold = false;
	m_w_pressed = false;
	m_s_pressed = false;
	m_a_pressed = false;
	m_d_pressed = false;
	m_q_pressed = false;
	m_e_pressed = false;
	m_mouse_left_drag = false;
	m_mouse_middle_drag = false;
	m_mouse_right_drag = false;
}

void Application::create() {
   compileShaders();

#ifdef PORSCHE_MESH
   PlyDataReader::getSingletonPtr()->readDataInfo("../data/big_porsche.ply", nullptr, 0);
#elif defined(SPHERE_MESH)
   PlyDataReader::getSingletonPtr()->readDataInfo("../data/sphere.ply", nullptr, 0);
#else
   PlyDataReader::getSingletonPtr()->readDataInfo("../data/bunny.ply", nullptr, 0);
#endif

   unsigned int nVertices = PlyDataReader::getSingletonPtr()->getNumVertices();
   unsigned int nFaces    = PlyDataReader::getSingletonPtr()->getNumFaces();


   vertices.resize(nVertices+4);
   indices.resize(nFaces * 3+6);
   
   PlyDataReader::getSingletonPtr()->readData(vertices.data(), indices.data());

   glm::vec3 center;
   glm::float32 min_y = vertices[0].pos.y;
   size_t i = 0;
   for (; i < vertices.size()-4; i++) {
     center += vertices[i].pos;
     min_y = glm::min(min_y, vertices[i].pos.y);
   }
   center /= vertices.size();

#ifdef ADD_GROUND
   float width = 400.0f;
   vertices[nVertices + 0].pos = glm::vec3(-width, min_y, -width);
   vertices[nVertices + 0].normal = glm::vec3(0, 1, 0);
   vertices[nVertices + 1].pos = glm::vec3(-width, min_y, width);
   vertices[nVertices + 1].normal = glm::vec3(0, 1, 0);
   vertices[nVertices + 2].pos = glm::vec3( width, min_y, -width);
   vertices[nVertices + 2].normal = glm::vec3(0, 1, 0);
   vertices[nVertices + 3].pos = glm::vec3( width, min_y, width);
   vertices[nVertices + 3].normal = glm::vec3(0, 1, 0);

   indices[3 * nFaces + 0] = nVertices + 0;
   indices[3 * nFaces + 1] = nVertices + 1;
   indices[3 * nFaces + 2] = nVertices + 2;
   indices[3 * nFaces + 3] = nVertices + 2;
   indices[3 * nFaces + 4] = nVertices + 1;
   indices[3 * nFaces + 5] = nVertices + 3;
#endif

   for (size_t i = 0; i < vertices.size(); i++) {
     vertices[i].pos -= center;
   }

   for (size_t i = 0; i < vertices.size(); i++) {

#ifdef PORSCHE_MESH
     vertices[i].pos *= 0.13;
#elif defined(SPHERE_MESH)
     vertices[i].pos *= m_arcBallRadius;
#else
     vertices[i].pos *= 5.0;
#endif
   }

   glGenBuffers(1, &vertices_buffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PlyObjVertex), vertices.data(), GL_STATIC_DRAW);

   glGenBuffers(1, &indices_buffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

   glGenBuffers(1, &mousePointerBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, mousePointerBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
}

void Application::update(float time, float timeSinceLastFrame) {

    //if (m_w_pressed)
    //    m_camera.Move(CameraDirection::FORWARD);

    //if (m_s_pressed)
    //    m_camera.Move(CameraDirection::BACK);

    //if (m_a_pressed)
    //    m_camera.Move(CameraDirection::LEFT);

    //if (m_d_pressed)
    //    m_camera.Move(CameraDirection::RIGHT);

    //if (m_q_pressed)
    //    m_camera.Move(CameraDirection::UP);

    //if (m_e_pressed)
    //    m_camera.Move(CameraDirection::DOWN);

    //// Updating the camera matrices
    //m_camera.Update();
    //m_camera.GetMatricies(m_projmat, m_viewmat, m_worldmat);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_transformation_buffer);
    glm::mat4* transform_matrices = (glm::mat4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 3 * sizeof(glm::mat4), GL_MAP_WRITE_BIT);
    transform_matrices[0] = m_navigation.getProjection();
    transform_matrices[1] = m_navigation.getView();
    transform_matrices[2] = glm::mat4(1.0f);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    // updating the lighting info
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_lighting_buffer);
    glm::vec4* light_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
    light_info[0] = glm::vec4(-1, -1, -1, 0);
    //light_info[1] = glm::vec4(m_camera.getPosition(), 1.0f);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    // Buffer 2 is reserved for the sample points of the Ambient Occlusion Rendering

    // updating the general information for every object
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_general_buffer);
    glm::vec4* general_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void Application::draw() {

  glViewport(0, 0, m_width, m_height);
  
  glClearColor(back_color[0], back_color[1], back_color[2], back_color[3]);
  glClearDepth(one);
    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawPly();

  drawMousePointer();
  
  // Draw the world coordinate system
  glViewport(0, 0, 100, 100);
  glUseProgram(m_coord_system_program);
  glDrawArrays(GL_LINES, 0, 6);
}

void Application::drawPly() {

  glUseProgram(ply_program);

  bool use_const_color = true;
  float const_color[] = { 1.0f, 1.0f, 1.0f };
  if (use_const_color) {
    glUniform1i(5, 1);
    glUniform3fv(6, 1, const_color);
  }
  else {
    glUniform1i(5, 0);
  }

  glUniform1i(0, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  int stride = sizeof(PlyObjVertex);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
  glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, stride, (char*)0);
  glVertexAttribPointer((GLint)1, 3, GL_FLOAT, GL_FALSE, stride, (char*)0 + 12);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);

  //std::cout << "Try to draw...\n";
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
  // std::cout << "Drawing done...\n";

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Application::setMousePosRepresentation(glm::vec3 mousePointerViewPos)
{
	glm::vec4 mousePointerWorldPos = m_navigation.getInvView() * glm::vec4(mousePointerViewPos, 1.0f);

#if 0
	std::cout << "mouse Pointer Pos = " << mousePointerViewPos[0] << " " << mousePointerViewPos[1] << " " << mousePointerViewPos[2] << std::endl;
#endif

	glBindBuffer(GL_ARRAY_BUFFER, mousePointerBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3), &mousePointerWorldPos);
}

void Application::drawMousePointer()
{
	//glDisable(GL_DEPTH_WRITEMASK);
	glEnableVertexAttribArray(0);
	glUseProgram(mousePointerDrawProgram);

	glUniform1f(0, 0.05f);

	glBindBuffer(GL_ARRAY_BUFFER, mousePointerBuffer);
	glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, (char*)0);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);
	//glEnable(GL_DEPTH_WRITEMASK);
	
}

float Application::readDepthBuffer(glm::dvec2 mousePos)
{
	float fDepth;
	glReadPixels(static_cast<GLint>(mousePos[0]), static_cast<GLint>(m_height - mousePos[1]), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fDepth);
	//glReadPixels(static_cast<GLint>(mousePos[0]), static_cast<GLint>(mousePos[1]), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fDepth);
	return fDepth;
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
    
  m_coord_system_program = compile_link_vs_fs("../glsl/coord_sys.vert", "../glsl/coord_sys.frag");
  ply_program = compile_link_vs_fs("../glsl/ply.vert", "../glsl/ply.frag");
  mousePointerDrawProgram = compile_link_vs_gs_fs("../glsl/mousePosShader.vert", "../glsl/mousePosShader.geom", "../glsl/mousePosShader.frag");

}

void Application::EventMouseButton(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Get the current mouse cursor position
		glm::dvec2 currentMousePos;
		glfwGetCursorPos(m_window, &currentMousePos[0], &currentMousePos[1]);

		glm::dmat4 viewMat = m_navigation.getView();
		glm::dvec3 viewCenterOfRotation(0.0);
		viewCenterOfRotation = glm::dvec3(viewMat * glm::dvec4(viewCenterOfRotation, 1.0));

		float depth = readDepthBuffer(currentMousePos);

		setMousePosRepresentation(m_navigation.getPointViewCoord(currentMousePos, readDepthBuffer(currentMousePos)));

		glm::dvec3 viewcurrentMousePos = m_navigation.getPointViewCoord(currentMousePos, depth);
		double viewSpaceRadius = glm::length(viewcurrentMousePos - viewCenterOfRotation);

//		setMousePosRepresentation(viewSpaceRadius);


#if 0
		std::cout << "=================================================================\n";
		std::cout << "   Initial State: \n";
		std::cout << "viewCenterOfRotation = " << viewCenterOfRotation[0] << " " << viewCenterOfRotation[1] << " " << viewCenterOfRotation[2] << std::endl;
		std::cout << "viewcurrentMousePos = " << viewcurrentMousePos[0] << " " << viewcurrentMousePos[1] << " " << viewcurrentMousePos[2] << std::endl;
		std::cout << "viewSpaceRadius = " << viewSpaceRadius << std::endl;
		std::cout << "=================================================================\n";
#endif

		m_navigation.startUpdate(currentMousePos, viewCenterOfRotation/*viewcurrentMousePos*/, viewSpaceRadius);
		m_mouse_left_drag = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		m_navigation.endUpdate();
		m_mouse_left_drag = false;
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glm::dvec2 currentMousePos;
		glfwGetCursorPos(m_window, &currentMousePos[0], &currentMousePos[1]);

		float depth = readDepthBuffer(currentMousePos);

		setMousePosRepresentation(m_navigation.getPointViewCoord(currentMousePos, readDepthBuffer(currentMousePos)));
		glm::dvec3 viewcurrentMousePos = m_navigation.getPointViewCoord(currentMousePos, depth);

		m_navigation.startPan(viewcurrentMousePos);
		m_mouse_right_drag = true;
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		m_navigation.endPan();
		m_mouse_right_drag = false;
	}

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    m_mouse_middle_drag = true;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    m_mouse_middle_drag = false;
}

void Application::EventMousePos(GLFWwindow* window, double xpos, double ypos) {
	glm::dvec2 newMousePos = { xpos, ypos };

	

  if (m_mouse_left_drag) {
	  m_navigation.updateRotate(newMousePos);

	  ;

	  m_prevMousePos = newMousePos;
  }

  if (m_mouse_right_drag) {  
	  m_navigation.updatePan(newMousePos);
  }

  /*if (m_mouse_right_drag) {
    m_camera.OffsetFrustum(static_cast<int>(xpos), static_cast<int>(ypos));
  }

  m_camera.SetMousePos(static_cast<int>(xpos), static_cast<int>(ypos));*/
}

void Application::EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {
  //m_camera.MoveForward(static_cast<int>(yoffset));
	if(yoffset)
	{
		glm::dvec2 currentMousePos;
		glfwGetCursorPos(m_window, &currentMousePos[0], &currentMousePos[1]);
		glm::ivec2 screenSize;
		glfwGetWindowSize(m_window, &screenSize[0], &screenSize[1]);
		currentMousePos[1] = screenSize[1] - currentMousePos[1];
		currentMousePos += .5;
		currentMousePos /= screenSize;
		currentMousePos = currentMousePos * 2. - 1.;
		m_navigation.zoomStep(yoffset < 0 ? -1 : 1, currentMousePos);
	}
}

void Application::EventKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS){
    if (key == GLFW_KEY_W)  m_w_pressed = true;
    if (key == GLFW_KEY_S)  m_s_pressed = true;
    if (key == GLFW_KEY_A)  m_a_pressed = true;
    if (key == GLFW_KEY_D)  m_d_pressed = true;
    if (key == GLFW_KEY_Q)  m_q_pressed = true;
    if (key == GLFW_KEY_E)  m_e_pressed = true;

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

    if (key == GLFW_KEY_LEFT_CONTROL)           m_controlKeyHold    = false;
    if (key == GLFW_KEY_LEFT_ALT)               m_altKeyHold        = false;

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    //m_camera.SetMousePos(static_cast<int>(xpos), static_cast<int>(ypos));
  }
}

void Application::EventChar(GLFWwindow * window, int codepoint)
{
}

// Callback function called by GLFW when window size changes
void Application::WindowSizeCB(GLFWwindow* window, int width, int height) {
  m_width = width; m_height = height;
  glViewport(0, 0, width, height);
  m_aspectRatio = static_cast<double>(width) / static_cast<double>(height);
  m_navigation.setProject(m_fov, m_aspectRatio, m_zNear);
  m_navigation.setScreenSize(m_width, m_height);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS){
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    //m_camera.Move2D(static_cast<int>(xpos), static_cast<int>(ypos));
    m_controlKeyHold = true;
  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE){
    m_controlKeyHold = false;
  }
}


// ============================================================================
// Static Global definition of GLFW handlers
// ============================================================================
void EventMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (auto self = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
		self->EventMouseButton(window, button, action, mods);
	}
}

void EventMousePos(GLFWwindow* window, double xpos, double ypos) {
	if (auto self = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
		self->EventMousePos(window, xpos, ypos);
	}
}

void EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {
	if (auto self = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
		self->EventMouseWheel(window, xoffset, yoffset);
	}
}

void EventKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (auto self = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
		self->EventKey(window, key, scancode, action, mods);
	}
}

void EventChar(GLFWwindow* window, int codepoint) {
	if (auto self = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
		self->EventChar(window, codepoint);
	}
}

void WindowSizeCB(GLFWwindow* window, int width, int height) {
	if (auto self = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
		self->WindowSizeCB(window, width, height);
	}
}

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (auto self = static_cast<Application*>(glfwGetWindowUserPointer(window))) {
		self->key_callback(window, key, scancode, action, mods);
	}
}