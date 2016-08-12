struct PointLight {
	vec3  point;
	vec4  color;
	float intensity;		// intensity of the light
	float inner_radius;     // inner radius of the light
	float outer_radius;     // outter radius of the light
};