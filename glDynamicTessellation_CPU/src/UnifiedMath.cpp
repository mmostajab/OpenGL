#include "UnifiedMath.h"

Vector4Df UnifiedMath::make_Vector4Df(Vector3Df v3d, float w)
{
#ifdef USE_OPENSG
    return Vector4Df(v3d.x(), v3d.y(), v3d.z(), w);
#else
    return Vector4Df(v3d, w);
#endif
}

float UnifiedMath::clamp(float value, float min, float max)
{
#ifdef USE_OPENSG
    if(value < min) return min;
    if(value > max) return max;
    return value;
#else 
    return glm::clamp(value, min, max);
#endif
}

float UnifiedMath::dot(Vector3Df a, Vector3Df b) 
{
#ifdef USE_OPENSG
    return a.dot(b);
#else
    return glm::dot(a, b);
#endif
}

Vector3Df UnifiedMath::cross(Vector3Df a, Vector3Df b)
{
#ifdef USE_OPENSG
  return a.dot(b);
#else
  return glm::cross(a, b);
#endif
}


float UnifiedMath::dot(Vector4Df a, Vector4Df b)
{
#ifdef USE_OPENSG
  return a.dot(b);
#else
  return glm::dot(a, b);
#endif
}

float UnifiedMath::length(Vector3Df v){
#ifdef USE_OPENSG
    return v.length();
#else
    return glm::length(v);
#endif
}


float UnifiedMath::length(Vector4Df v) {
#ifdef USE_OPENSG
  return v.length();
#else
  return glm::length(v);
#endif
}

Vector3Df UnifiedMath::max(Vector3Df a, Vector3Df b){
#ifdef USE_OPENSG
    return Vector3Df(osg::osgMax(a[0], b[0]), osg::osgMax(a[1], b[1]), osg::osgMax(a[2], b[2]));
#else
    return glm::max(a, b);
#endif
}

Vector4Df UnifiedMath::max(Vector4Df a, Vector4Df b){
#ifdef USE_OPENSG
    return Vector4Df(osg::osgMax(a[0], b[0]), osg::osgMax(a[1], b[1]), osg::osgMax(a[2], b[2]), osg::osgMax(a[3], b[3]));
#else
    return glm::max(a, b);
#endif
}

float UnifiedMath::max(float a, float b){
#ifdef USE_OPENSG
    return OSG::osgMax(a, b);
#else
    return glm::max(a, b);
#endif
  }

Vector3Df UnifiedMath::min(Vector3Df a, Vector3Df b){
#ifdef USE_OPENSG
    return Vector3Df(osg::osgMin(a[0], b[0]), osg::osgMin(a[1], b[1]), osg::osgMin(a[2], b[2]));
#else
    return glm::min(a, b);
#endif
}

Vector4Df UnifiedMath::min(Vector4Df a, Vector4Df b){
#ifdef USE_OPENSG
    return Vector4Df(osg::osgMin(a[0], b[0]), osg::osgMin(a[1], b[1]), osg::osgMin(a[2], b[2]), osg::osgMin(a[3], b[3]));
#else
    return glm::min(a, b);
#endif
}

Vector4Df UnifiedMath::matrixMultiply(Matrix4x4f m, Vector4Df v){
#ifdef USE_OPENSG
    Vector4Df r;
    //Vector4Df r;
    //m.mult(v, r);
    //m.multLeftFull(Vector3Df(v[0], v[1], v[2]), r);

    r[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3]; 
    r[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3]; 
    r[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3]; 
    r[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3]; 
     
    return r;
#else
    return m * v;
#endif
}

Matrix4x4f UnifiedMath::matrixMultiply(Matrix4x4f m1, Matrix4x4f m2) {
#ifdef USE_OPENSG
  Matrix4x4f r = m1;
  r.mult(m2);
  return r;
#else
  return m1 * m2;
#endif
}

Matrix4x4f UnifiedMath::matrixSequenceMultiply(const std::vector<Matrix4x4f>& matrixSequence)
{
  Matrix4x4f res = getIdentityMatrix();
  for (size_t i = 0; i< matrixSequence.size(); i++) {
    const Matrix4x4f& m = matrixSequence[i];
    res = matrixMultiply(res, m);
  }
  return res;
}

Matrix4x4f UnifiedMath::getIdentityMatrix()
{
#ifdef USE_OPENSG
  return OSG::Matrix4f::identity();
#else
  return glm::mat4(1.0f);
#endif
}

Matrix4x4f UnifiedMath::translate(Vector3Df v)
{
#ifdef USE_OPENSG
  OSG::Matrix4f m;
  m.setIdentity();
  m.setTranslate(v);
  return m;
#else
  return glm::translate<float>(v);
#endif // USE_OPENSG
}

Matrix4x4f UnifiedMath::rotate(float angle, Vector3Df axis)
{
#ifdef USE_OPENSG
  OSG::Matrix4f m;
  m.setIdentity();
  m.setRotate(OSG::Quaternion(axis, angle));
  return m;
#else
  return glm::rotate<float>(angle, axis);
#endif // USE_OPENSG
}

Matrix4x4f UnifiedMath::scale(Vector3Df s)
{
#ifdef USE_OPENSG
  OSG::Matrix4f m;
  m.setIdentity();
  m.setScale(s);
  return m;
#else
  return glm::scale<float>(s);
#endif // USE_OPENSG
}

Vector3Df UnifiedMath::TransformWithTranslation(Matrix4x4f m, Vector3Df v)
{
  Vector4Df v4d = make_Vector4Df(v, 1.0f);
  Vector4Df res = matrixMultiply(m, v4d);
  return Vector3Df(res[0], res[1], res[2]);
}
