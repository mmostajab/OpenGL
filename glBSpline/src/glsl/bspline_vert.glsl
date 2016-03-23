#version 430 core

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 control_point0;
layout( location = 2 ) in vec3 control_point1;

layout(std140, binding = 0) uniform TransformBlock {
    vec3 points[];
};

out VS_OUT{
	vec4 color;
	vec4 control_point0;
	vec4 control_point1;
} vs_out;

void main() {
    gl_Position = proj_mat * view_mat * world_mat * vec4(position, 1.0f);
	vs_out.control_point0 = proj_mat * view_mat * world_mat * vec4(control_point0, 1.0f);
	vs_out.control_point1 = proj_mat * view_mat * world_mat * vec4(control_point1, 1.0f);
}