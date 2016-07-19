#version 430 core

layout (location = 0) uniform mat4 mv;

uniform vec4 position[6] = { vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 0.0f) };
uniform vec3 color[3] = { vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f) };

out vec4 vec_color;

void main() {
	gl_Position =  vec4((mv * position[gl_VertexID]).xyz, 1.0f);
	vec_color = vec4(color[gl_VertexID / 2], 1.0f);
}