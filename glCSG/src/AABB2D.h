

/**
*
* AABB2D
*
* Two-dimensional axis-aligned bounding box.
*
*/

#pragma once

#include <algorithm>
#include <limits>

#ifdef WIN32
#    define AABB2D_INLINE __forceinline
#else
#    define AABB2D_INLINE inline
#endif

class AABB2D
{
public:

  /// Initialize with an invalid box.
  AABB2D_INLINE AABB2D();

  /// Initialize with new min and max vectors.
  AABB2D_INLINE AABB2D(const float min[2], const float max[2]);

  /// Check if the box is valid (i.e., min < max).
  AABB2D_INLINE bool valid() const;

  /// Check if the box contains a 2D point.
  AABB2D_INLINE bool contains(const float point[2]) const;

  /// Extend the box with a 2D point.
  AABB2D_INLINE void extend(const float x, const float y);

  /// Extend the box with a 2D point.
  AABB2D_INLINE void extend(const float point[2]);

  /// Extend the box with another box.
  AABB2D_INLINE void extend(const AABB2D &aabb);

  /// Enlarge min and max by delta.
  AABB2D_INLINE void enlarge(const float delta);

  /// Get average extent.
  AABB2D_INLINE float getAvgExtent() const;

  /// Get min extent.
  AABB2D_INLINE float getMinExtent() const;

  /// Get max extent.
  AABB2D_INLINE float getMaxExtent() const;

  /// Bounding min and max vectors.
  float min[2];
  float max[2];
};

// ****************************************************************************

AABB2D_INLINE AABB2D::AABB2D()
{
  min[0] = min[1] = +std::numeric_limits<float>::max();
  max[0] = max[1] = -std::numeric_limits<float>::max();
}

AABB2D_INLINE AABB2D::AABB2D(const float min[2], const float max[2])
{
  this->min[0] = min[0];
  this->min[1] = min[1];

  this->max[0] = max[0];
  this->max[1] = max[1];
}

AABB2D_INLINE bool AABB2D::valid() const
{
  return    min[0] <= max[0]
    && min[1] <= max[1];
}

AABB2D_INLINE bool AABB2D::contains(const float point[2]) const
{
  return    point[0] >= min[0] && point[0] <= max[0]
    && point[1] >= min[1] && point[1] <= max[1];
}

AABB2D_INLINE void AABB2D::extend(const float x, const float y)
{
  min[0] = std::min(min[0], x);
  min[1] = std::min(min[1], y);

  max[0] = std::max(max[0], x);
  max[1] = std::max(max[1], y);
}

AABB2D_INLINE void AABB2D::extend(const float point[2])
{
  min[0] = std::min(min[0], point[0]);
  min[1] = std::min(min[1], point[1]);

  max[0] = std::max(max[0], point[0]);
  max[1] = std::max(max[1], point[1]);
}

AABB2D_INLINE void AABB2D::extend(const AABB2D &aabb)
{
  min[0] = std::min(min[0], aabb.min[0]);
  min[1] = std::min(min[1], aabb.min[1]);

  max[0] = std::max(max[0], aabb.max[0]);
  max[1] = std::max(max[1], aabb.max[1]);
}

AABB2D_INLINE void AABB2D::enlarge(const float delta)
{
  min[0] -= delta;
  min[1] -= delta;

  max[0] += delta;
  max[1] += delta;
}

AABB2D_INLINE float AABB2D::getAvgExtent() const
{
  return 1.0f / 2.0f * (max[0] - min[0] + max[1] - min[1]);
}

AABB2D_INLINE float AABB2D::getMinExtent() const
{
  return std::min(max[0] - min[0], max[1] - min[1]);
}

AABB2D_INLINE float AABB2D::getMaxExtent() const
{
  return std::max(max[0] - min[0], max[1] - min[1]);
}
