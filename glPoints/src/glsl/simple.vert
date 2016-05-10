#version 430 core

layout(std140, binding = 0) uniform TransformBlock {
    mat4 proj_mat;
    mat4 view_mat;
    mat4 world_mat;
};

layout(std140, binding = 1) uniform LightBlock {
    vec4 dir;
    vec4 pos;
} ;

in vec3 position;
in vec3 color;

out vec4 pixel_color;

void main() {
    gl_Position = proj_mat * view_mat * world_mat * vec4(position, 1.0f);
	pixel_color = vec4(color, 1.0f);
}