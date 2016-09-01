#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef USE_OPENSG

  #include <OpenSG/OSGVec3fFields.h>
  #include <OpenSG/OSGMatrix.h>
  #include <OpenSG/OSGMathIO.h>

  #define Vector3D   OSG::Vec3f
  #define Vector4D   OSG::Vec4f
  #define Matrix4x4  OSG::Matrix4f

#else

  // GLM
  #include <glm/glm.hpp>
  #include <glm/gtx/transform.hpp>
  #include <glm/gtc/matrix_transform.hpp>

  #define Vector3D  glm::vec3
  #define Vector4D  glm::vec4
  #define Matrix4x4 glm::mat4

#endif

// STD
#include <vector>

class UnifiedMath {

public:

  // make vectors
  static Vector4D make_Vector4D(Vector3D v3d, float w);

  // clamp
  static float clamp(float value, float min, float max);

  // dot
  static float dot(Vector3D a, Vector3D b);
  static float dot(Vector4D a, Vector4D b);

  // length
  static float length(Vector3D v);
  static float length(Vector4D v);
  
  // max
  static Vector3D max(Vector3D a, Vector3D b);
  static Vector4D max(Vector4D a, Vector4D b);
  static float max(float a, float b);

  // min
  static Vector3D min(Vector3D a, Vector3D b);
  static Vector4D min(Vector4D a, Vector4D b);
  
  // Matrix operations
  static Vector4D  matrixMultiply(Matrix4x4 m, Vector4D v);
  static Matrix4x4 matrixMultiply(Matrix4x4 m1, Matrix4x4 m2);
  static Matrix4x4 matrixSequenceMultiply(const std::vector<Matrix4x4>& matrixSequence);

  static Matrix4x4 getIdentityMatrix();
  static Matrix4x4 translate(Vector3D v);
  static Matrix4x4 rotate(float angle, Vector3D axis);
  static Matrix4x4 scale(Vector3D s);

  // Trasnformation
  static Vector3D TransformWithTranslation(Matrix4x4 m, Vector3D v);
};


#endif