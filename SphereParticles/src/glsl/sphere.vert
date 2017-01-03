#version 450

layout(std140, binding = 0) uniform TransformBlock {
    mat4 proj_mat;
    mat4 view_mat;
    mat4 world_mat;
	mat4 projViewWorld_mat;
} ;

layout(location = 0) in vec3 pos;

void main()
{
  gl_Position = projViewWorld_mat * vec4(pos, 1);
}