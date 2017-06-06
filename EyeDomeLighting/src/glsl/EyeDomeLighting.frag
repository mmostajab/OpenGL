#version 430 core

layout(location = 0) uniform vec3 lightDir = vec3(1.0f);

// Samplers for pre-rendered color, normal and depth
layout (binding = 0) uniform sampler2D sColor;
layout (binding = 1) uniform sampler2D sDepth;
//layout (binding = 1) uniform sampler2D sNormalDepth;

layout(location = 0) out vec4 out_color;

//#define SHOW_COLOR
//#define GRADIENT_NORMALS
#define EYE_DOME_LIGHTING

void main() {
	
	vec2 texSize = textureSize(sColor, 0);
	vec2 P = gl_FragCoord.xy / texSize;
	float delta = 1.0f / max(texSize.x, texSize.y); 

	vec4 color = textureLod(sColor, P, 0);
	vec4 D = textureLod(sDepth, P, 0);

	ivec3 offset = ivec3(-1, 0, +1);

#ifdef GRADIENT_NORMALS
	vec3 n = normalize(
		vec3(
			-(textureOffset(sDepth, P, offset.zy).x - textureOffset(sDepth, P, offset.zx).x) / (2.0f * delta),
			-(textureOffset(sDepth, P, offset.yz).x - textureOffset(sDepth, P, offset.yx).x) / (2.0f * delta),
			1.0f
		)
	);
	float diffuse = dot(lightDir, n);
#endif

#ifdef EYE_DOME_LIGHTING
	float edlStrength = 90;
	int radius = 1;
	float response = 
		max(0, log2(texture(sDepth, P).x) - log2(textureOffset(sDepth, P, radius * offset.zy).x)) + 
		max(0, log2(texture(sDepth, P).x) - log2(textureOffset(sDepth, P, radius * offset.zx).x)) +
		max(0, log2(texture(sDepth, P).x) - log2(textureOffset(sDepth, P, radius * offset.yz).x)) + 
		max(0, log2(texture(sDepth, P).x) - log2(textureOffset(sDepth, P, radius * offset.yx).x)) ;
	response /= 4.0;

	float diffuse = exp(-response*300.0*edlStrength);
#endif

#ifndef SHOW_COLOR
	out_color = vec4(diffuse*color.xyz, 1.0f);
#else
	out_color = vec4(color.xyz, 1.0f);
#endif
	gl_FragDepth = D.x;
}