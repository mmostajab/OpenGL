#version 430 core

uniform sampler3D tex;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 inv_view_mat;
uniform mat4 world_mat;
uniform vec3 camera_pos;


in VS_OUT{
	vec4 color;
} fs_in;

layout( location = 0 ) uniform vec3 color = vec3(1.0f, 0.0f, 1.0f);

layout( location = 0 ) out vec4 out_color;

void main() {

  out_color = fs_in.color;

}
