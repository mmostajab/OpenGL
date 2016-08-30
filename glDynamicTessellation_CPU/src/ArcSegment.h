#ifndef __ARC_SEGMENT_H__
#define __ARC_SEGMENT_H__

#ifdef OPENSG_VECTORS

#else

// Windows
#ifdef _WIN32
  #include <Windows.h>
#endif

  // GL
  #include <gl/glew.h>

  // GLM
  #include <glm\glm.hpp>

#endif

// STD
#include <vector>

// Dynamic Tessellation
#include "DynTessArcPrimitive.h"

extern float mult;

class ArcSegment : public DynTessArcPrimitive
{	
public:
  ArcSegment(
    glm::vec3 _p1     = glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3 _p2     = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 _center = glm::vec3(0.0f)/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs        = 5);

  void createBuffer()                                               override; 
  void updateBuffer(glm::mat4 mvp, unsigned int w, unsigned int h)  override;
  void draw()                                                       override;

//protected:
public:
    glm::vec3 	p1;               //      ___       ;
    glm::vec3 	p2;               //    /     \     ;
    glm::vec3   center;           // p1 ------- p2  ;
  
#ifdef OPENSG_VECTORS

#else

  GLuint buffer = 0;
  GLint  nVertices;
  int    nSegs = -1;

#endif

};

#endif