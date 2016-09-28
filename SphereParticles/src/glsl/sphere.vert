#version 120

void main()
{
  gl_Position = gl_Vertex;
  gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
}