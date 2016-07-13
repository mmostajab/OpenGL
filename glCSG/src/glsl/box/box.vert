#version 450 core

layout ( location = 0 ) uniform mat4 mvp;

layout ( location = 0 ) in vec3  position;
layout ( location = 1 ) in vec3  dims;
layout ( location = 2 ) in int   sign;

out VS_OUT {

	vec3  dims;
	float sign;

} vs_out;

void main() {
	gl_Position   = vec4(position, 1.0f);
	vs_out.dims   = dims;
	vs_out.sign   = float(sign);
} 