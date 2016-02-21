#version 430 core

// #define MY_DEBUG

// Samplers for pre-rendered color, normal and depth
layout (binding = 0) uniform sampler2D sColor;
layout (binding = 1) uniform sampler2D sNormalDepth;

layout (location = 0) uniform int ssao_active = 1;

// Final output
layout (location = 0) out vec4 color;

void main(void) {

	// Get texture position from gl_FragCoord
  vec2 P = gl_FragCoord.xy / textureSize(sColor, 0);

	// Get object color from color texture
  vec4 object_color = textureLod(sColor, P, 0);
	color = object_color;
}