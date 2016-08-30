#include "ArcPrimitiveHelper.h"

#include <complex>

glm::float32 ArcPrimitiveHelper::calcCurveLength(const glm::vec3 & _p0, const glm::vec3 & _p1, const glm::vec3 & _center)
{
  // the angle between _p0 - _center and _p1 - _center
  glm::float32 alpha =
    acosf(
      glm::clamp(
        glm::dot(_p0 - _center, _p1 - _center)
        / (glm::length(_p0 - _center) * glm::length(_p1 - _center))
        , -1.0f, 1.0f
      ) 
    );

  glm::float32 radius = glm::max(glm::length(_p0 - _center), glm::length(_p1 - _center));

  return radius * static_cast<float>(alpha);
}

glm::float32 ArcPrimitiveHelper::calcProjectedCurveLength(
  const glm::mat4& mvp, const uint32_t& w, const uint32_t& h,
  const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec3 & center
)
{
  glm::float32 curve_length = 0.0f;

  glm::vec4 p1_proj = mvp * glm::vec4(p0, 1.0f);
  glm::vec4 p2_proj = mvp * glm::vec4(p1, 1.0f);
  glm::vec4 center_proj = mvp * glm::vec4(center, 1.0f);

  p1_proj.x /= p1_proj.z;	    p1_proj.y /= p1_proj.z;
  p2_proj.x /= p2_proj.z;	    p2_proj.y /= p2_proj.z;
  center_proj.x /= center_proj.z; center_proj.y /= center_proj.z;

  float radius = glm::max(glm::length(p1_proj - center_proj) * w / 2.0f, glm::length(p2_proj - center_proj) * h / 2.0f); //glm::length(len) / (2 * sin(static_cast<float>(alpha)));
  float alpha =
    acosf(
      glm::clamp(
        glm::dot(p1_proj - center_proj, p2_proj - center_proj)
        / (glm::length(p1_proj - center_proj) * glm::length(p2_proj - center_proj)), -1.0f, 1.0f
      ));

  curve_length = radius * static_cast<float>(alpha) + 1;

  return curve_length;
}

glm::vec3 ArcPrimitiveHelper::slerp(
  const glm::vec3& _p0, const glm::vec3 & _p1, 
  const glm::float32& _thetha, const glm::float32 & _angle)
{
  return 
    sinf(_angle - _thetha) / sinf(static_cast<float>(_angle)) * _p0 +
    sinf(_thetha) / sinf(static_cast<float>(_angle))          * _p1;
}

glm::vec3 ArcPrimitiveHelper::interpolation_complex(
  const glm::vec3& _p0, const glm::vec3& _p1, 
  const glm::float32& _thetha, const glm::float32& _angle)
{
  std::complex<float> numerator = (1.f - (std::complex<float>(cos(_thetha), sin(_thetha))));
  std::complex<float> divisor   = (1.f - std::complex<float>(cos(_angle), sin(_angle)));
  std::complex<float> w         = numerator / divisor;

  std::complex<float> p_complex = (1.f - w) * std::complex<float>(_p1.x, _p1.y) + w * std::complex<float>(_p0.x, _p0.y);
  glm::vec3 p = glm::vec3(p_complex.real(), p_complex.imag(), 0.0f);

  return p;
}
