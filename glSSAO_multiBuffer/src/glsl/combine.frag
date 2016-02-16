#version 430 core

// Samplers for pre-rendered color, normal and depth
layout (binding = 0) uniform sampler2D sColor;
layout (binding = 1) uniform sampler2D sNormalDepth;
layout (binding = 2) uniform sampler2D sDepth;

// Final output
layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal_depth;

void main(void) {
  
  vec2 P_color       = gl_FragCoord.xy / textureSize(sColor, 0);
  vec2 P_normalDepth = gl_FragCoord.xy / textureSize(sNormalDepth, 0);
  vec2 P_depth       = gl_FragCoord.xy / textureSize(sDepth, 0);

  color         =  texture(sColor, P_color);
  normal_depth  =  texture(sNormalDepth, P_normalDepth);
  gl_FragDepth  =  texture(sDepth, P_depth).x;
}