#version 430 core

in vec4 pixel_color;

layout (location = 0) out vec4 out_color;

void main() {
	out_color = pixel_color;
}