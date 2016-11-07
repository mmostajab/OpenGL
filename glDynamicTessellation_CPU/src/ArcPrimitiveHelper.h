#ifndef __ARC_PRIMITIVE_HELPER_H__
#define __ARC_PRIMITIVE_HELPER_H__

#include "UnifiedMath.h"

// STD
#include <stdint.h>

#ifdef USE_OPENSG

#else

// Widnows
//#include <windows.h>

// GL
#include <GL/glew.h>

#endif

// Vertex of a Dynamic Tessellated Arc Primitive.
struct Vertex {
  Vector3Df position;
};

class ArcPrimitiveHelper {
public:
  // Projection-to-Clip Coordinate
  static Vector4Df Projection2ClipCoords(Vector4Df v);

  // calculates the curve length
  static float calcCurveLength(
    const Vector3Df& p0, const Vector3Df& p1, const Vector3Df& center
  );

  // calculates the projected curve length
  static float calcProjectedCurveLength(
    const Matrix4x4f& mvp, const uint32_t& w, const uint32_t& h,
    const Vector3Df& p0, const Vector3Df& p1, const Vector3Df& center
  );

  // spherical linear interpolation
  static Vector3Df    slerp(
                                const Vector3Df& p0, const Vector3Df& p1,
                                const float& thetha, const float& angle
  );

  // interpolates between p0 and p1 using the complex rep.
  static Vector3Df    interpolation_complex(
                                const Vector3Df& p0, const Vector3Df& p1,
                                const float& thetha, const float& angle
  );

  // returns the angle of a vector with positive x in CCW order
  static float       angle_with_positive_x(const Vector3Df& a);

  // returns the cosine of the angle between vectors @a and @b
  static float       cos_angle_between( const Vector3Df& a, const Vector3Df& b );

  // returns the cosine of the angle between vectors @a and @b
  static float       angle_between(const Vector3Df& a, const Vector3Df& b);

  static void        produceCurvePoints(
    const Vector3Df & p1, const Vector3Df & p2, const Vector3Df& center, 
    int nSegs, bool ccw,
    std::vector<Vertex>& vertices);
};

#endif // __ARC_PRIMITIVE_HELPER_H__