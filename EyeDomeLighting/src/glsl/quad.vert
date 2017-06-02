#version 430 core

uniform vec4 position[4] = { 
	vec4(-1.0f, -1.0f, 0.0f, 1.0f), 
	vec4(-1.0f,  1.0f, 0.0f, 1.0f), 
	vec4( 1.0f, -1.0f, 0.0f, 1.0f), 
	vec4( 1.0f,  1.0f, 0.0f, 1.0f)
};

void main() {
	gl_Position = position[gl_VertexID];
}