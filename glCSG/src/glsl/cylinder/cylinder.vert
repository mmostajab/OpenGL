#version 450 core

layout ( location = 0 ) uniform mat4 mvp;

layout ( location = 0 ) in vec3  position;
layout ( location = 1 ) in float radius;
layout ( location = 2 ) in float height;

out VS_OUT {

	float radius;
	float height;

} vs_out;

void main() {
	gl_Position   = mvp * vec4(position, 1.0f);
	vs_out.radius = radius;
	vs_out.height = height;
} 