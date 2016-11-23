#include "ArcPrimitiveHelper.h"

#include <complex>

Vector4Df ArcPrimitiveHelper::Projection2ClipCoords(Vector4Df v)
{
#ifdef USE_OPENSG
  return Vector4Df(v[0] / v[3], v[1] / v[3], v[2] / v[3], 1.0f);
#else
  return Vector4Df(v[0] / v.w, v[1] / v.w, v[2] / v.w, 1.0f);
#endif
}

float ArcPrimitiveHelper::calcCurveLength(const Vector3Df & _p0, const Vector3Df & _p1, const Vector3Df & _center)
{
  // the angle between _p0 - _center and _p1 - _center
  float alpha =
    acosf(
      UnifiedMath::clamp(
        UnifiedMath::dot(_p0 - _center, _p1 - _center)
          / (UnifiedMath::length(_p0 - _center) * UnifiedMath::length(_p1 - _center))
          , -1.0f, 1.0f
      ) 
    );

  float radius = UnifiedMath::max(UnifiedMath::length(_p0 - _center), UnifiedMath::length(_p1 - _center));

  return radius * static_cast<float>(alpha);
}

float ArcPrimitiveHelper::calcProjectedCurveLength(
  const Matrix4x4f& mvp, const uint32_t& w, const uint32_t& h,
  const Vector3Df& p0, const Vector3Df& p1, const Vector3Df & center
)
{
  float curve_length = 0.0f;

  Vector4Df p1_proj = mvp * UnifiedMath::make_Vector4Df(p0, 1.0f);
  Vector4Df p2_proj = mvp * UnifiedMath::make_Vector4Df(p1, 1.0f);
  Vector4Df center_proj = mvp * UnifiedMath::make_Vector4Df(center, 1.0f);

  p1_proj     = Projection2ClipCoords(p1_proj);
  p2_proj     = Projection2ClipCoords(p2_proj);
  center_proj = Projection2ClipCoords(center_proj);

  float radius =  UnifiedMath::max(UnifiedMath::length(p1_proj - center_proj) * w / 2.0f, UnifiedMath::length(p2_proj - center_proj) * h / 2.0f); //glm::length(len) / (2 * sin(static_cast<float>(alpha)));
  float alpha =
    acosf(
      UnifiedMath::clamp(
        UnifiedMath::dot(p1_proj - center_proj, p2_proj - center_proj)
        / (UnifiedMath::length(p1_proj - center_proj) * UnifiedMath::length(p2_proj - center_proj)), -1.0f, 1.0f
      ));

  curve_length = radius * static_cast<float>(alpha) + 1;

  return curve_length;
}

Vector3Df ArcPrimitiveHelper::slerp(
  const Vector3Df& _p0, const Vector3Df & _p1, 
  const float& _thetha, const float & _angle)
{
  return 
    sinf(_angle - _thetha) / sinf(static_cast<float>(_angle)) * _p0 +
    sinf(_thetha) / sinf(static_cast<float>(_angle))          * _p1;
}

Vector3Df ArcPrimitiveHelper::interpolation_complex(
  const Vector3Df& _p0, const Vector3Df& _p1, 
  const float& _thetha, const float& _angle)
{
  std::complex<float> numerator = (1.f - (std::complex<float>(cos(_thetha), sin(_thetha))));
  std::complex<float> divisor   = (1.f - std::complex<float>(cos(_angle), sin(_angle)));
  std::complex<float> w         = numerator / divisor;

  std::complex<float> p_complex = (1.f - w) * std::complex<float>(_p1[0], _p1[1]) + w * std::complex<float>(_p0[0], _p0[1]);
  Vector3Df p = Vector3Df(p_complex.real(), p_complex.imag(), 0.0f);

  return p;
}

float ArcPrimitiveHelper::angle_with_positive_x(const Vector3Df & a)
{
  float l = sqrtf(a[0] * a[0] + a[1] * a[1]);
  if (a[1] >= 0) {
    return acos(a[0] / l);
  }
  else {
    return 2 * UnifiedMath::pi() - acos(a[0] / l);
  }
}

float ArcPrimitiveHelper::cos_angle_between( const Vector3Df& a, const Vector3Df& b )
{
  return UnifiedMath::clamp(UnifiedMath::dot(a, b) / (UnifiedMath::length(a) * UnifiedMath::length(b)), -1.0f, 1.0f);
}

float ArcPrimitiveHelper::angle_between(const Vector3Df & a, const Vector3Df & b)
{
  float absloute_angle_a = angle_with_positive_x(a);
  float absloute_angle_b = angle_with_positive_x(b);

  float thetha = acos(cos_angle_between(a, b));

  // Check if the angle is clockwise
  Vector3Df c = UnifiedMath::cross(a, b);
  if (c[2] <= 0)
    return thetha;
  else
    return 2 * UnifiedMath::pi() - thetha;
}

void ArcPrimitiveHelper::produceCurvePoints(
  const Vector3Df & p1, const Vector3Df & p2, const Vector3Df& center, int nSegs, bool ccw,
  std::vector<Vertex>& vertices)
{
  Vertex v;

  Vector3Df a = p1 - center;
  Vector3Df b = p2 - center;
  float alpha = ArcPrimitiveHelper::angle_between(a, b);

  for (int i = 0; i < nSegs + 1; i++) {
    float t = static_cast<float>(i) / static_cast<float>(nSegs);
    float thetha = ccw ? (1 - t) * alpha : t * alpha;

    Vector3Df p;
#ifdef USE_SLERP
    p = ArcPrimitiveHelper::slerp(a, b, thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
    p = ArcPrimitiveHelper::interpolation_complex(a, b, thetha, alpha);
#endif

    v.position = p + center;
    vertices.push_back(v);
  }
}
