#ifndef __DEFINES_H__
#define __DEFINES_H__

#define USE_OPENSG
#ifdef USE_OPENSG

  #include <OpenSG/OSGVec3fFields.h>
  #include <OpenSG/OSGMatrix.h>
  #include <OpenSG/OSGMathIO.h>
  #include <OpenSG/OSGLinearTransform.h>
  #include <OpenSG/OSGQuaternion.h>


  #define Vector3Df   OSG::Vec3f
  #define Vector4Df   OSG::Vec4f
  #define Matrix4x4f  OSG::Matrix4f

#else

  // GLM
  #include <glm/glm.hpp>
  #include <glm/gtx/transform.hpp>
  #include <glm/gtc/matrix_transform.hpp>

  #define Vector3Df  glm::vec3
  #define Vector4Df  glm::vec4
  #define Matrix4x4f glm::mat4

#endif

// STD
#include <vector>

class UnifiedMath {

public:

  // make vectors
  static Vector4Df make_Vector4Df(Vector3Df v3d, float w);

  // clamp
  static float clamp(float value, float min, float max);

  // dot
  static float dot(Vector3Df a, Vector3Df b);
  static float dot(Vector4Df a, Vector4Df b);

  // length
  static float length(Vector3Df v);
  static float length(Vector4Df v);
  
  // max
  static Vector3Df max(Vector3Df a, Vector3Df b);
  static Vector4Df max(Vector4Df a, Vector4Df b);
  static float max(float a, float b);

  // min
  static Vector3Df min(Vector3Df a, Vector3Df b);
  static Vector4Df min(Vector4Df a, Vector4Df b);
  
  // Matrix operations
  static Vector4Df  matrixMultiply(Matrix4x4f m, Vector4Df v);
  static Matrix4x4f matrixMultiply(Matrix4x4f m1, Matrix4x4f m2);
  static Matrix4x4f matrixSequenceMultiply(const std::vector<Matrix4x4f>& matrixSequence);

  static Matrix4x4f getIdentityMatrix();
  static Matrix4x4f translate(Vector3Df v);
  static Matrix4x4f rotate(float angle, Vector3Df axis);
  static Matrix4x4f scale(Vector3Df s);

  // Trasnformation
  static Vector3Df TransformWithTranslation(Matrix4x4f m, Vector3Df v);
};


#endif