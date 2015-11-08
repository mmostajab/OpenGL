#version 430 core

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 inv_view_mat;
uniform mat4 world_mat;
uniform vec3 camera_pos;

uniform vec3 dims;

in VS_OUT{
	vec3 localpos;
	vec4 color;
} fs_in;

layout( location = 0 ) uniform vec3 color = vec3(1.0f, 0.0f, 1.0f);
layout (location = 0) out vec4 out_color;

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

  float t_f_min = max(t_min.x, max(t_min.y, t_min.z));
  float t_f_max = min(t_max.x, min(t_max.y, t_max.z));
  
  //out_color = vec4(fs_in.localpos, 1.0f);
  out_color = vec4((org + t_f_max * dir) / dims + vec3(0.5f), 1.0f);
	//out_color = fs_in.color;
}
