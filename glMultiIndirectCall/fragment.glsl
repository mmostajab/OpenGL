#version 430 core

in VS_OUT{
  vec4 color;
} fs_in;

layout( location = 0 ) uniform vec3 color = vec3(1.0f, 0.0f, 1.0f);

layout (location = 0) out vec4 out_color;



void main() {
  out_color = fs_in.color;
}