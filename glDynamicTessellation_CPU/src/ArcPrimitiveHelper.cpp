#include "ArcPrimitiveHelper.h"

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

glm::vec3 ArcPrimitiveHelper::slerp(
  const glm::vec3& _p0, const glm::vec3 & _p1, 
  const glm::float32& _thetha, const glm::float32 & _angle)
{
  return 
    sinf(_angle - _thetha) / sinf(static_cast<float>(_angle)) * _p0 +
    sinf(_thetha) / sinf(static_cast<float>(_angle))          * _p1;
}
