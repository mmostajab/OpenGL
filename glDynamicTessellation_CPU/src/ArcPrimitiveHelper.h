#ifndef __ARC_PRIMITIVE_HELPER_H__
#define __ARC_PRIMITIVE_HELPER_H__

#include "UnifiedMath.h"

// STD
#include <stdint.h>

#ifdef USE_OPENSG

#else

// Widnows
#include <windows.h>

// GL
#include <GL/glew.h>

#endif


class ArcPrimitiveHelper {
public:
  // Projection-to-Clip Coordinate
  static Vector4D Projection2ClipCoords(Vector4D v);

  // calculates the curve length
  static float calcCurveLength(
    const Vector3D& p0, const Vector3D& p1, const Vector3D& center
  );

  // calculates the projected curve length
  static float calcProjectedCurveLength(
    const Matrix4x4& mvp, const uint32_t& w, const uint32_t& h,
    const Vector3D& p0, const Vector3D& p1, const Vector3D& center
  );

  // spherical linear interpolation
  static Vector3D    slerp(
                                const Vector3D& p0, const Vector3D& p1,
                                const float& thetha, const float& angle
  );

  // interpolates between p0 and p1 using the complex rep.
  static Vector3D    interpolation_complex(
                                const Vector3D& p0, const Vector3D& p1,
                                const float& thetha, const float& angle
  );

  // returns the angle between vectors @a and @b
  static float       angle_between( const Vector3D& a, const Vector3D& b );
};

#endif // __ARC_PRIMITIVE_HELPER_H__