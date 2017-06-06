#version 430 core

layout(std140, binding = 0) uniform TransformBlock {
		mat4 proj_mat;
		mat4 view_mat;
		mat4 world_mat;
	} ;

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal; 

out VS_OUT {
	vec4 worldCoord;
	vec3 viewNormal;
} vs_out;

void main() {
  gl_Position =  proj_mat * view_mat * world_mat * vec4( position, 1.0f );
  vs_out.worldCoord = world_mat * vec4(position, 1.0f);
  vs_out.viewNormal = mat3(view_mat * world_mat) * normal;
}