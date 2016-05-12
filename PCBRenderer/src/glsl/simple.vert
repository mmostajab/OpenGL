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

uniform float time;

void main() {
    mat4 rotation_mat = 
		mat4(
			sin(time), 0.0f, cos(time), 0.0f,
			0.0f,      1.0f, 0.0f,      0.0f, 
	       -cos(time), 0.0f, sin(time), 0.0f,
		   0.0f,       0.0f, 0.0f,      1.0f
		);

    gl_Position = proj_mat * view_mat * world_mat * rotation_mat * vec4(position, 1.0f);
	pixel_color = vec4(color, 1.0f);
}