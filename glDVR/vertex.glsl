#version 430 core

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;
layout( location = 2 ) in vec3 local_position;


out VS_OUT{
  vec3 localpos;
  vec4 color;
} vs_out;

void main() {
    gl_Position = proj_mat * view_mat * world_mat * vec4(position, 1.0f);
    //vs_out.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vs_out.color = vec4(color, 1.0f);
	vs_out.localpos = position; //local_position;
	//vs_out.local_pos = local_position;
}