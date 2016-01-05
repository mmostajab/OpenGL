#version 430 core

layout(std140, binding = 0) uniform TransformBlock {
    mat4 proj_mat;
    mat4 view_mat;
    mat4 world_mat;
} ;

layout(std140, binding = 1) uniform LightBlock {
    vec4 dir;
    vec4 pos;
} ;

layout (location = 5) uniform int use_const_color;
layout (location = 6) uniform vec3 const_color = vec3(1.0f, 0.0f, 0.0f);

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;
layout( location = 2 ) in vec3 normal;

out VS_OUT {
    vec3 view_dir_view;
    vec3 light_dir_view;
    vec3 normal_view;
    vec4 color;
	vec4 world_pos;
} vs_out;

void main() {
    vec4 p_view = view_mat * world_mat * vec4(position, 1.0f);

    vs_out.normal_view = mat3(view_mat * world_mat) * normal;
    vs_out.light_dir_view = mat3(view_mat * world_mat) * dir.xyz;
    vs_out.view_dir_view = -p_view.xyz;
    if(use_const_color == 0)
		vs_out.color = vec4(color, 1.0f);
	else
		vs_out.color = vec4(const_color, 1.0f);
    //vs_out.color = vec4(normalize(normal), 1.0f);
	vs_out.world_pos = world_mat * vec4(position, 1.0f);

    gl_Position   = proj_mat * p_view;
}