#version 430 core

in vec2 tex_coord;

layout (location = 0) out vec4 out_color;

layout (binding = 0) uniform sampler2D tex;

void main() {
	out_color = texture(tex, tex_coord); //vec4(tex_coord, 0, 1);
}