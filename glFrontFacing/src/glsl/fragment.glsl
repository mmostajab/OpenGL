#version 330 core

in VS_OUT{
  vec4 color;
} fs_in;

uniform vec3 color = vec3(1.0f, 0.0f, 1.0f);
out vec4 out_color;

void main() {

  if(gl_FrontFacing)
    out_color = vec4(0, 1, 0, 1);
  else
    out_color = vec4(1, 0, 0, 1);

  return;
  
  out_color = fs_in.color;
}