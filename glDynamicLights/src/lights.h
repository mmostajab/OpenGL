#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include <glm/glm.hpp>

namespace Light{

	struct PointLight {
		glm::vec4  point;
		glm::vec4  color;
		float	   intensity;		// intensity of the light
		float      inner_radius;     // inner radius of the light
		float	   outer_radius;     // outter radius of the light
	};

}

#endif