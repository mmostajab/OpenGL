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
  // calculates the curve length
  static glm::float32 calcCurveLength(
    const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& center
  );

  // calculates the projected curve length
  static glm::float32 calcProjectedCurveLength(
    const glm::mat4& mvp, const uint32_t& w, const uint32_t& h,
    const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& center
  );

  // spherical linear interpolation
  static glm::vec3    slerp(
                                const glm::vec3& p0, const glm::vec3& p1,
                                const glm::float32& thetha, const glm::float32& angle
  );

  // interpolates between p0 and p1 using the complex rep.
  static glm::vec3    interpolation_complex(
                                const glm::vec3& p0, const glm::vec3& p1,
                                const glm::float32& thetha, const glm::float32& angle
  );
#endif
};

#endif // __ARC_PRIMITIVE_HELPER_H__