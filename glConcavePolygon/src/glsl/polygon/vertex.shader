#version 450 core

layout ( location = 0 ) uniform mat4 mvp;

layout ( location = 0 ) in vec3  position;
layout ( location = 1 ) in int   sign;

out VS_OUT {

	vec3 normal;
	float  sign;

} vs_out;

void main() {
	gl_Position   = mvp * vec4(position, 1.0f);
	vs_out.normal = vec3(0.0f, 1.0f, 0.0f);
	vs_out.sign   = float(sign);
} 