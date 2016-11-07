#ifndef __ANT_TWEAK_BAR_GUI__
#define __ANT_TWEAK_BAR_GUI__

#include <AntTweakBar.h>
#include <GLFW/glfw3.h>

class AntTweakBarGUI {
public:
  AntTweakBarGUI();

  void init(const unsigned int& width, const unsigned int& height);
  void draw();
  void shutdown();

  ~AntTweakBarGUI();

  static void TwEventMouseButtonGLFW3(int button, int action, int mods);
  static void TwEventMousePosGLFW3(double xpos, double ypos);
  static void TwEventMouseWheelGLFW3(double xoffset, double yoffset);
  static void TwEventKeyGLFW3(int key, int scancode, int action, int mods);
  static void TwEventCharGLFW3(int codepoint);

  static void putBar(const char* barName, float xPercent, float yPercent, float xSizePercent, float ySizePercent);

  // Callback function called by GLFW when window size changes
  static void WindowSizeCB(int width, int height);
  static void repositionBars();
  //private:
public:
  static unsigned int m_width, m_height;

  // general
  bool  general_wireframe;
  bool  general_wireframe_pre;
  bool  general_frustumCulling;
  bool  general_frustumCulling_pre;
  bool  general_increaseTriangles;
  bool  general_decreaseTriangles;
  float general_triangulationAccuracyFactor;  // triangulation accuracy
  float general_dropCullingFactor;            // drop culling factor
  bool  general_writeEvaluations;
  bool  general_clearEvaluations;
  bool  general_renderAABBs;
  
  // status
  float status_overall_fps;
  int   status_nTriangles;
  float status_perf_triangles_per_second;
  float status_frustum_culling_time;
  float status_triangulation_time;
  float status_draw_time;
  int   status_nVisible_primitives;
  int   status_nInvisible_primitives;

};

#endif