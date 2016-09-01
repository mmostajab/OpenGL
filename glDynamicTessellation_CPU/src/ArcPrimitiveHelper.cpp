#include "ArcPrimitiveHelper.h"

#include <complex>

Vector4D ArcPrimitiveHelper::Projection2ClipCoords(Vector4D v)
{
#ifdef USE_OPENSG
  return Vector4D(v.x() / v.w(), v.y() / v.w(), v.z() / v.w(), 1.0f);
#else
  return Vector4D(v.x / v.w, v.y / v.w, v.z / v.w, 1.0f);
#endif
}

float ArcPrimitiveHelper::calcCurveLength(const Vector3D & _p0, const Vector3D & _p1, const Vector3D & _center)
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
  const Matrix4x4& mvp, const uint32_t& w, const uint32_t& h,
  const Vector3D & p0, const Vector3D & p1, const Vector3D & center
)
{
  float curve_length = 0.0f;

  Vector4D p1_proj = mvp * UnifiedMath::make_Vector4D(p0, 1.0f);
  Vector4D p2_proj = mvp * UnifiedMath::make_Vector4D(p1, 1.0f);
  Vector4D center_proj = mvp * UnifiedMath::make_Vector4D(center, 1.0f);

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

Vector3D ArcPrimitiveHelper::slerp(
  const Vector3D& _p0, const Vector3D & _p1, 
  const float& _thetha, const float & _angle)
{
  return 
    sinf(_angle - _thetha) / sinf(static_cast<float>(_angle)) * _p0 +
    sinf(_thetha) / sinf(static_cast<float>(_angle))          * _p1;
}

Vector3D ArcPrimitiveHelper::interpolation_complex(
  const Vector3D& _p0, const Vector3D& _p1, 
  const float& _thetha, const float& _angle)
{
  std::complex<float> numerator = (1.f - (std::complex<float>(cos(_thetha), sin(_thetha))));
  std::complex<float> divisor   = (1.f - std::complex<float>(cos(_angle), sin(_angle)));
  std::complex<float> w         = numerator / divisor;

  std::complex<float> p_complex = (1.f - w) * std::complex<float>(_p1[0], _p1[1]) + w * std::complex<float>(_p0[0], _p0[1]);
  Vector3D p = Vector3D(p_complex.real(), p_complex.imag(), 0.0f);

  return p;
}

float ArcPrimitiveHelper::angle_between( const Vector3D& a, const Vector3D& b )
{
  return UnifiedMath::clamp(UnifiedMath::dot(a, b) / (UnifiedMath::length(a) * UnifiedMath::length(b)), -1.0f, 1.0f);
}