#version 430 core

// Samplers for pre-rendered color, normal and depth
layout (binding = 0) uniform sampler2D sColor;
layout (binding = 1) uniform sampler2D sDepth;
//layout (binding = 1) uniform sampler2D sNormalDepth;

layout(location = 0) out vec4 out_color;

void main() {

	vec2 P = gl_FragCoord.xy / textureSize(sColor, 0);

	vec4 C = textureLod(sColor, P, 0);
	vec4 D = textureLod(sDepth, P, 0);

	out_color = vec4(C.xyz, 1);
	gl_FragDepth = D.x;
}