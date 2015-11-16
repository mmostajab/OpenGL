#version 430 core

uniform sampler3D tex;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 inv_view_mat;
uniform mat4 world_mat;
uniform vec3 camera_pos;

uniform float min_range = 0.9f;
uniform float max_range = 1.0f;

uniform float min_alpha = 0.2f;
uniform float max_alpha = 0.6f;

uniform float stepsize = 0.001f;

uniform vec3 dims;

in VS_OUT{
	vec3 localpos;
	vec4 color;
} fs_in;

layout( location = 0 ) uniform vec3 color = vec3(1.0f, 0.0f, 1.0f);
layout (location = 0) out vec4 out_color;

bool inRange(float val, float min, float max)
{
  return (val >= min) && (val <= max);
}

float getInterpVal(const float s, const float min_s, const float max_s, const float min_value, const float max_value){
  float v = ((s - min_s) / (max_s - min_s));

  return (1.0f - v) * min_value + v * max_value;
}

vec3 getJetRGBMap(const float value, const float min_value, const float max_value)
{
  float v = ((value - min_value) / (max_value - min_value));

  float r, g, b;

  // Setting Red Component
  if(v >= 0 && v <= 0.35f)
  {
    r = 0;
  }
  else if(v > 0.35f && v <= 0.66f)
  {
    r = ((v - 0.35f) * 1 + (0.66f - v) * 0) / (0.66f - 0.35f);
  }
  else if(v > 0.66f && v <= 0.89f)
  {
    r = 1.0;
  }
  else
  {
    r = ((v - 0.89f) * 0.5f + (1 - v) * 1) / (1 - 0.89f) ;
  }

  // Setting Green Component
  if(v >= 0 && v <= 0.125f)
  {
    g = 0;
  }
  else if(v > 0.125f && v <= 0.375f)
  {
    g = ((v - 0.125f) * 1 + (0.375f - v) * 0) / (0.25f);
  }
  else if(v > 0.375f && v <= 0.64f)
  {
    g = 1.0;
  }
  else if(v > 0.64f && v <= 0.91f)
  {
    g = ((v - 0.64f) * 0 + (0.91f - v) * 1) / (0.91f - 0.64f);
  }
  else
  {
    g = 0;
  }

  // Setting Blue Component
  if(v >= 0 && v <= 0.11f)
  {
    b = ((v - 0) * 1 + (0.11f - v) * 0.5f) / (0.11);
  }
  else if(v > 0.11f && v <= 0.34f)
  {
    b = 1.0;
  }
  else if(v > 0.34f && v <= 0.65f)
  {
    b = ((v - 0.34f) * 0 + (0.65f - v) * 1) / (0.65f - 0.34f);
  }
  else
  {
    b = 0;
  }

  return vec3(r, g, b);
}

void main() {

  vec3 pos = fs_in.localpos; //(fs_in.localpos - vec3(0.5f)) * dims;
  vec3 org = camera_pos;
  vec3 dir = normalize(pos - org);

  vec3 t_min = ((-dims/2.0f) - org) / dir;
  vec3 t_max = (dims/2.0f - org)    / dir;

  vec3 tmp0 = min(t_min, t_max);
  vec3 tmp1 = max(t_min, t_max);

  t_min = tmp0;
  t_max = tmp1;

  float t_entryPoint = max(t_min.x, max(t_min.y, t_min.z));
  float t_exitPoint  = min(t_max.x, min(t_max.y, t_max.z));

	
  vec3  texCoords;//= (org + t_exitPoint * dir) / dims + vec3(0.5f);

  float t = t_entryPoint;
  float t_prev = t;
 
  float alpha = 0;
  vec3 color = vec3(0.0f);
  while(t < t_exitPoint)
  {
	   texCoords = (org + t * dir) / dims + vec3(0.5f);
	   float s = texture(tex, texCoords).r;

    if(inRange(s, min_range, max_range)){

      float alphaF = getInterpVal(s, min_range, max_range, min_alpha, max_alpha);
      vec3  colorF = getJetRGBMap(s, min_range, max_range);

      alpha = alpha + (1 - alpha) * alphaF;
      color = color + (1 - alpha) * colorF;

      if(abs(alpha - 1) < 0.001f)
        break;
    }

	  t += stepsize;
  }

  out_color = vec4(color, alpha);

}
