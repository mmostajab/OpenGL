#include "UnifiedMath.h"

Vector4D UnifiedMath::make_Vector4D(Vector3D v3d, float w)
{
#ifdef USE_OPENSG
    return Vector4D(v3d.x(), v3d.y(), v3d.z(), w);
#else
    return Vector4D(v3d, w);
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

float UnifiedMath::dot(Vector3D a, Vector3D b) 
{
#ifdef USE_OPENSG
    return a.dot(b);
#else
    return glm::dot(a, b);
#endif
}


float UnifiedMath::dot(Vector4D a, Vector4D b)
{
#ifdef USE_OPENSG
  return a.dot(b);
#else
  return glm::dot(a, b);
#endif
}

float UnifiedMath::length(Vector3D v){
#ifdef USE_OPENSG
    return v.length();
#else
    return glm::length(v);
#endif
}


float UnifiedMath::length(Vector4D v) {
#ifdef USE_OPENSG
  return v.length();
#else
  return glm::length(v);
#endif
}

Vector3D UnifiedMath::max(Vector3D a, Vector3D b){
#ifdef USE_OPENSG
    return Vector3D(osg::osgMax(a[0], b[0]), osg::osgMax(a[1], b[1]), osg::osgMax(a[2], b[2]));
#else
    return glm::max(a, b);
#endif
}

Vector4D UnifiedMath::max(Vector4D a, Vector4D b){
#ifdef USE_OPENSG
    return Vector4D(osg::osgMax(a[0], b[0]), osg::osgMax(a[1], b[1]), osg::osgMax(a[2], b[2]), osg::osgMax(a[3], b[3]));
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

Vector3D UnifiedMath::min(Vector3D a, Vector3D b){
#ifdef USE_OPENSG
    return Vector3D(osg::osgMin(a[0], b[0]), osg::osgMin(a[1], b[1]), osg::osgMin(a[2], b[2]));
#else
    return glm::min(a, b);
#endif
}

Vector4D UnifiedMath::min(Vector4D a, Vector4D b){
#ifdef USE_OPENSG
    return Vector4D(osg::osgMin(a[0], b[0]), osg::osgMin(a[1], b[1]), osg::osgMin(a[2], b[2]), osg::osgMin(a[3], b[3]));
#else
    return glm::min(a, b);
#endif
}

Vector4D UnifiedMath::matrixMultiply(Matrix4x4 m, Vector4D v){
#ifdef USE_OPENSG
    Vector4D r;
    m.mult(v, r);
    return r;
#else
    return m * v;
#endif
}

Matrix4x4 UnifiedMath::matrixMultiply(Matrix4x4 m1, Matrix4x4 m2) {
#ifdef USE_OPENSG
  Matrix4x4 r = m1;
  r.mult(m2);
  return r;
#else
  return m1 * m2;
#endif
}

Matrix4x4 UnifiedMath::matrixSequenceMultiply(const std::vector<Matrix4x4>& matrixSequence)
{
  Matrix4x4 res = getIdentityMatrix();
  for (size_t i = 0; i< matrixSequence.size(); i++) {
    const Matrix4x4& m = matrixSequence[i];
    res = matrixMultiply(res, m);
  }
  return res;
}

Matrix4x4 UnifiedMath::getIdentityMatrix()
{
#ifdef USE_OPENSG
  return OSG::Matrix4f::identity();
#else
  return glm::mat4(1.0f);
#endif
}

Matrix4x4 UnifiedMath::translate(Vector3D v)
{
#ifdef USE_OPENSG
  OSG::Matrix4f m;
  m.setIdentity();
  m.setTranslate(v);
  return m;
#else
  return glm::translate<float>(v);
#endif // _DEBUG
}

Matrix4x4 UnifiedMath::rotate(float angle, Vector3D axis)
{
#ifdef USE_OPENSG
  OSG::Matrix4f m;
  m.setIdentity();
  m.setRotate(OSG::Quaternion(axis, angle));
  return m;
#else
  return glm::rotate<float>(angle, axis);
#endif // 
}

Matrix4x4 UnifiedMath::scale(Vector3D s)
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

Vector3D UnifiedMath::TransformWithTranslation(Matrix4x4 m, Vector3D v)
{
  Vector4D v4d = make_Vector4D(v, 1.0f);
  Vector4D res = matrixMultiply(m, v4d);
  return Vector3D(res[0], res[1], res[2]);
}
