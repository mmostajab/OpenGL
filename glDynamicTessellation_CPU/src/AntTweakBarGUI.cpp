#include "AntTweakBarGUI.h"

// STD
#include <iostream>

unsigned int AntTweakBarGUI::m_width = 800;
unsigned int AntTweakBarGUI::m_height = 600;

AntTweakBarGUI::AntTweakBarGUI() {
  m_width = -1;
  m_height = -1;
}

void AntTweakBarGUI::init(const unsigned int& width, const unsigned int& height) {
  m_width = width;    m_height = height;

  if (!TwInit(TwGraphAPI::TW_OPENGL, NULL))
    exit(EXIT_FAILURE);

  TwWindowSize(m_width, m_height);
  TwBar* generalBar = TwNewBar("General");
  TwDefine("General position='480 400'");
  TwDefine("General color='176 176 176'");
  TwDefine("General size='300 50'");
  TwAddVarRW(generalBar, "Tri. Accuracy", TW_TYPE_FLOAT, &general_triangulationAccuracyFactor, "min=0.1 step=0.1");
  TwAddVarRW(generalBar, "Drop Cull Factor", TW_TYPE_FLOAT, &general_dropCullingFactor, "min=0.1 step=0.1");
  TwAddVarRW(generalBar, "Write Evaluations", TW_TYPE_BOOL8, &general_writeEvaluations, "");

  TwBar* statusBar = TwNewBar("Status");
  TwDefine("Status position='480 480'");
  TwDefine("Status size='300 100'");
  TwDefine("Status color='176 176 176'");
  TwAddVarRO(statusBar, "Triangles", TW_TYPE_UINT32, &status_nTriangles, "");
  TwAddVarRO(statusBar, "Tri. per sec", TW_TYPE_FLOAT, &status_perf_triangles_per_second, "");
  TwAddVarRO(statusBar, "Triangulation time (ms)", TW_TYPE_FLOAT, &status_triangulation_time, "");
  //TwAddVarRO(statusBar, "Avg. lines per sec", TW_TYPE_FLOAT, &status_avg_lines_per_sec, "");
  //TwAddVarRO(statusBar, "FPS", TW_TYPE_FLOAT, &status_fps, "");
}

void AntTweakBarGUI::draw() {
  TwDraw();
}

void AntTweakBarGUI::shutdown() {
  TwTerminate();
}

AntTweakBarGUI::~AntTweakBarGUI() {

}

void AntTweakBarGUI::TwEventMouseButtonGLFW3(int button, int action, int mods) {
  TwEventMouseButtonGLFW(button, action);
}

void AntTweakBarGUI::TwEventMousePosGLFW3(double xpos, double ypos) {
  TwMouseMotion(int(xpos), int(ypos));
}

void AntTweakBarGUI::TwEventMouseWheelGLFW3(double xoffset, double yoffset) {
  TwEventMouseWheelGLFW(int(yoffset));
}

void AntTweakBarGUI::TwEventKeyGLFW3(int key, int scancode, int action, int mods) {
  TwEventKeyGLFW(key, action);
}

void AntTweakBarGUI::TwEventCharGLFW3(int codepoint) {
  TwEventCharGLFW(codepoint, GLFW_PRESS);
}

void AntTweakBarGUI::putBar(const char * barName, float xPercent, float yPercent, float xSizePercent, float ySizePercent)
{
  char cmd[1000];

  int posX, posY;
  posX = static_cast<int>(xPercent * m_width);
  posY = static_cast<int>(yPercent * m_height);

  int sizeX, sizeY;
  sizeX = static_cast<int>(xSizePercent * m_width);
  sizeY = static_cast<int>(ySizePercent * m_height);

  sprintf_s(cmd, "%s position='%i %i'", barName, posX, posY);
  TwDefine(cmd);

  sprintf_s(cmd, "%s size='%i %i'", barName, sizeX, sizeY);
  TwDefine(cmd);
}

// Callback function called by GLFW when window size changes
void AntTweakBarGUI::WindowSizeCB(int width, int height) {
  // Send the new window size to AntTweakBar
  TwWindowSize(width, height);
  m_width = width;
  m_height = height;

  putBar("General", 480.0f / 800.0f, 420.0f / 600.0f, 300.0f / 800.0f, 50.0f / 600.0f);
  putBar("Status", 480.0f / 800.0f, 480.0f / 600.0f, 300.0f / 800.0f, 100.0f / 600.0f);
}