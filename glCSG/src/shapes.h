#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <glm/glm.hpp>

#include "helper.h"
#include "AABB.h"

// ===================================================================
// ===================================================================
// == Directed Box
// ===================================================================
// ===================================================================

struct DirectedBox {
	glm::vec3       start, end;
	float           width, height;
	unsigned int	  geomID;
	unsigned int    shapeID;
	int				      sign;

	void set(const AABB& bbox) {
		start = bbox.min;
		end   = bbox.max;
	}
};

// ===================================================================
// ===================================================================
// == Box
// ===================================================================
// ===================================================================

struct Box {
    AABB			    bbox;
  unsigned int	  geomID;
  unsigned int    shapeID;
  int				      sign;

  void set(const AABB& bbox) {
    this->bbox = bbox;
  }

  glm::vec3 center() const { return (bbox.min + bbox.max) / 2.0f; }
  glm::vec3 dims()   const { return bbox.max - bbox.min; }
};

// ===================================================================
// ===================================================================
// == Cylinder
// ===================================================================
// ===================================================================

struct Cylinder {
	AABB			      bbox;
	unsigned int	  geomID;
	unsigned int    shapeID;
	int				      sign;

	void set(const AABB& bbox) {
		this->bbox = bbox;
	}

	void set(const glm::vec3& center, const float& r, const float& h) {
		bbox.min = center - glm::vec3(r, h / 2.0f, r);
		bbox.max = center + glm::vec3(r, h / 2.0f, r);
		//bbox.max.w = bbox.min.w = 1.0f;
	}

	inline glm::vec3 center()  const { return (bbox.min + bbox.max) / 2.0f; };
	inline float          radius()  const { return (bbox.max[0] - bbox.min[0]) / 2.0f; }
	inline float          height()  const { return bbox.max[1] - bbox.min[1]; }
};

// ===================================================================
// ===================================================================
// == Polygons
// ===================================================================
// ===================================================================

struct PolygonShape {
	int             start_idx, size;
	AABB            bbox;
	unsigned int	  geomID;
	unsigned int    shapeID;
	int				      sign;
};
 
#endif // __SHAPES_H__