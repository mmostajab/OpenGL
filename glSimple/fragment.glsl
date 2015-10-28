#version 330 core

in VS_OUT{
  vec4 color;
} fs_in;

uniform vec3 color = vec3(1.0f, 0.0f, 1.0f);
out vec4 out_color;

void main() {
  out_color = fs_in.color;
}