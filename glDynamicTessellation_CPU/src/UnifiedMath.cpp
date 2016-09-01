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

Vector3D UnifiedMath::TransformWithTranslation(Matrix4x4 m, Vector3D v)
{
  Vector4D v4d = make_Vector4D(v, 1.0f);
  Vector4D res = matrixMultiply(m, v4d);
  return Vector3D(res[0], res[1], res[2]);
}
