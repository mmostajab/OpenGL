#ifndef __ARC_PRIMITIVE_HELPER_H__
#define __ARC_PRIMITIVE_HELPER_H__

#ifndef OPENSG_VECTORS
// GLM
#include <glm/glm.hpp>
#endif

class ArcPrimitiveHelper {
public:
#ifdef OPENSG_VECTORS

#else
  static glm::float32 calcCurveLength(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& center);
  static glm::vec3    slerp(const glm::vec3& p0, const glm::vec3& p1, const glm::float32& thetha, const glm::float32& angle);
#endif
};

#endif // __ARC_PRIMITIVE_HELPER_H__