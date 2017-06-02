#version 430 core

layout(location = 0) in vec4 vec_color;

layout(location = 0) out vec4 out_color;

void main() {
  out_color = vec_color;
}