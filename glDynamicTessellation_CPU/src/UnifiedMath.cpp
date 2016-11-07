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

float UnifiedMath::min(float a, float b) {
#ifdef USE_OPENSG
  return OSG::osgMin(a, b);
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

float UnifiedMath::pi()
{
  return 3.1415926535897932384626433832795028841971693993751f;
}

float UnifiedMath::Radian2Degree(float rad) {
  return rad / pi() * 180.0f;
}

float UnifiedMath::Degree2Radian(float deg) {
  return deg / 180.0f * pi();
}

float UnifiedMath::inZeroTwoPiRange(float a) {
  const float twoPi = (2.0f * pi());
  if (a == 0) return twoPi;
  return a - floorf(a / twoPi) *  twoPi;
}

void UnifiedMath::sinBoundsCW(float angle0, float angle1, float& minVal, float& maxVal) {

  float sinAngle0 = sin(angle0);
  float sinAngle1 = sin(angle1);

  if (sinAngle0 <= sinAngle1) {
    //minAngle = angle0;
    //maxAngle = angle1;
    minVal = sinAngle0;
    maxVal = sinAngle1;
  }
  else {
    //minAngle = angle1;
    //maxAngle = angle0;
    minVal = sinAngle1;
    maxVal = sinAngle0;
  }

  // TODO Check
  {
    float start = inZeroTwoPiRange(angle0 - pi() / 2.0f);
    float end = inZeroTwoPiRange(angle1 - pi() / 2.0f);

    if (start <= end) {
      //maxAngle = 1.0f * pi() / 2.0f;
      maxVal = 1.0f;
    }
  }

  // TODO Check
  {
    float start = inZeroTwoPiRange(angle0 + pi() / 2.0f);
    float end = inZeroTwoPiRange(angle1 + pi() / 2.0f);

    if (start <= end) {
      //minAngle = 3.0f * pi() / 2.0f;
      minVal = -1.0f;
    }
  }

}

void UnifiedMath::cosBoundsCW(float angle0, float angle1, float& minVal, float& maxVal) {

  float cosAngle0 = glm::cos(angle0);
  float cosAngle1 = glm::cos(angle1);

  if (cosAngle0 <= cosAngle1) {
    //minAngle = angle0;
    //maxAngle = angle1;
    minVal = cosAngle0;
    maxVal = cosAngle1;
  }
  else {
    //minAngle = angle1;
    //maxAngle = angle0;
    minVal = cosAngle1;
    maxVal = cosAngle0;
  }

  // TODO Wrong
  {
    float start = inZeroTwoPiRange(angle0);
    float end = inZeroTwoPiRange(angle1);

    if (start <= end) {
      //minAngle = 0.0f;
      maxVal = 1.0f;
    }
  }

  // TODO Wrong
  {
    float start = inZeroTwoPiRange(angle0 + pi());
    float end = inZeroTwoPiRange(angle1 + pi());

    if (start <= end) {
      //minAngle = pi();
      minVal = -1.0f;
    }
  }

}

float UnifiedMath::angleWithPositiveX(Vector3Df v) {
  float acos_angle = v.x / sqrt(v.x * v.x + v.y * v.y);

  if (v.y >= 0) {
    return acos(acos_angle);
  }
  else {
    return 2.0f * pi() - acos(acos_angle);
  }
}

void UnifiedMath::curveMinMaxPoints(Vector3Df p0, Vector3Df p1, Vector3Df center, Vector3Df& minPoint, Vector3Df& maxPoint) {
  Vector3Df a = p0 - center;
  Vector3Df b = p1 - center;

  float angle0 = angleWithPositiveX(a);
  float angle1 = angleWithPositiveX(b);

  float minCosVal, maxCosVal, minSinVal, maxSinVal;

  cosBoundsCW(angle0, angle1, minCosVal, maxCosVal);
  //std::cout << "Cos: minVal = " << minCosVal << " maxVal = " << maxCosVal << std::endl;

  sinBoundsCW(angle0, angle1, minSinVal, maxSinVal);
  //std::cout << "Sin: minVal = " << minSinVal << " maxVal = " << maxSinVal << std::endl;

  float r0 = glm::length(p0 - center), r1 = glm::length(p1 - center);

  if (r0 >= r1) {
    minPoint[0] = center[0] + r0 * minCosVal;
    maxPoint[0] = center[0] + r0 * maxCosVal;

    minPoint[1] = center[1] + r0 * minSinVal;
    maxPoint[1] = center[1] + r0 * maxSinVal;
    
    minPoint[2] = center[2] + UnifiedMath::min(p0[2], UnifiedMath::min(p1[2], center[2]));
    maxPoint[2] = center[2] + UnifiedMath::max(p0[2], UnifiedMath::max(p1[2], center[2]));
  }
  else {
    minPoint[0] = center[0] + r1 * minCosVal;
    maxPoint[0] = center[0] + r1 * maxCosVal;

    minPoint[1] = center[1] + r1 * minSinVal;
    maxPoint[1] = center[1] + r1 * maxSinVal;

    minPoint[2] = center[2] + UnifiedMath::min(p0[2], UnifiedMath::min(p1[2], center[2]));
    maxPoint[2] = center[2] + UnifiedMath::max(p0[2], UnifiedMath::max(p1[2], center[2]));
  }
}

Frustum UnifiedMath::getFrustum(const Matrix4x4f& mvp) {
  Frustum frustum;

  // Left clipping plane.
  frustum.planes[0].set(mvp[0][3] + mvp[0][0], mvp[1][3] + mvp[1][0], mvp[2][3] + mvp[3][0], (mvp[3][3] + mvp[3][0] ) );
  // Right clipping plane.
  frustum.planes[1].set(mvp[0][3] - mvp[0][0], mvp[1][3] - mvp[1][0], mvp[2][3] - mvp[3][0], (mvp[3][3] - mvp[3][0] ) );

  // Bottom clipping plane.
  frustum.planes[2].set(mvp[0][3] + mvp[0][1], mvp[1][3] + mvp[1][1], mvp[2][3] + mvp[3][1], (mvp[3][3] + mvp[3][1]) );
  // Top clipping plane.
  frustum.planes[3].set(mvp[0][3] - mvp[0][1], mvp[1][3] - mvp[1][1], mvp[2][3] - mvp[3][1], (mvp[3][3] - mvp[3][1] ) );

  // Near clipping plane.
  frustum.planes[4].set(mvp[0][3] + mvp[0][2], mvp[1][3] + mvp[1][2], mvp[2][3] + mvp[3][2], (mvp[3][3] + mvp[3][2]));
  // Far clipping plane.
  frustum.planes[5].set(mvp[0][3] - mvp[0][2], mvp[1][3] - mvp[1][2], mvp[2][3] - mvp[3][2], (mvp[3][3] - mvp[3][2]));


  // Normalize, this is not always necessary...
  for (unsigned int i = 0; i < 6; i++)
  {
    frustum.planes[i].normalize();
  }

  return frustum;
}

bool UnifiedMath::FrustumAABBIntersect(const Frustum& frustum, const AABB & aabb)
{
  // We only need to do 6 point-plane tests
  for (int i = 0; i < 6; ++i)
  {
    // This is the current plane
    const Plane &p = frustum.planes[i];

    // p-vertex selection (with the index trick)
    // According to the plane normal we can know the
    // indices of the positive vertex
    const float px = (p.n[0] >= 0.0f) ? aabb.max[0] : aabb.min[0];
    const float py = (p.n[1] >= 0.0f) ? aabb.max[1] : aabb.min[1];
    const float pz = (p.n[2] >= 0.0f) ? aabb.max[2] : aabb.min[2];

    // Dot product
    // project p-vertex on plane normal
    // (How far is p-vertex from the origin)
    const float dp =
      (p.n[0] * px) +
      (p.n[1] * py) +
      (p.n[2] * pz);

    // Doesn't intersect if it is behind the plane
    if (dp < -p.d) { return false; }
  }

  return true;
}
