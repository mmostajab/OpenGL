#version 450 core

layout (location = 1) uniform vec3 eye;

layout (binding = 0, r32ui) 		uniform uimage2D head_pointer_image;
layout (binding = 1, rgba32ui) 		uniform writeonly uimageBuffer list_buffer;
layout (binding = 0, offset = 0) 	uniform atomic_uint list_counter;

layout(location = 0) out vec4 color;

in GS_OUT {
  vec3  normalized_pos;
  vec3  center;
  float radius;
  float height;
  float sign;
} fs_in;

void main(){
  vec3 normal;
	vec4 frag_color = vec4( (normal + vec3(1.0f)) / 2.0f, 1.0f );

	float front_facing = +1.0f;
	if(!gl_FrontFacing) front_facing = -1.0f;
  
  vec3 eyeLocalSpace = (eye - fs_in.center) / vec3(fs_in.radius, fs_in.height / 2.0f, fs_in.radius);
  vec3 dirLocalSpace = normalize(fs_in.normalized_pos - eyeLocalSpace);

  vec3 t_max_cands = (vec3( 1.0f) - eyeLocalSpace) / dirLocalSpace;
  vec3 t_min_cands = (vec3(-1.0f) - eyeLocalSpace) / dirLocalSpace;

  vec3 t_max = max(t_max_cands, t_min_cands);
  vec3 t_min = min(t_max_cands, t_min_cands);

  float t_entry = max(max(t_min.x, t_min.y), t_min.z);
  float t_exit  = min(min(t_max.x, t_max.y), t_max.z);

  vec3 entry_point = eyeLocalSpace + t_entry * dirLocalSpace;
  vec3 exit_point  = eyeLocalSpace + t_exit  * dirLocalSpace;
  
  if (abs(abs(entry_point.y) - 1.0f) < 0.0001f) {
    float r = entry_point.x*entry_point.x + entry_point.z*entry_point.z;
    if (r <= 1.0f) {
      color = vec4(0.5f, 1.0f, 0.5f, 1.0f);
      return;
    }
  }

  float a = (dirLocalSpace.x * dirLocalSpace.x + dirLocalSpace.z * dirLocalSpace.z);
  float b = /*2.0f **/ (eyeLocalSpace.x * dirLocalSpace.x + eyeLocalSpace.z * dirLocalSpace.z);
  float c = (eyeLocalSpace.x * eyeLocalSpace.x + eyeLocalSpace.z * eyeLocalSpace.z - 1.0f);

  float delta = b*b - a*c;

  if (delta >= 0.0f) {

    float t[2] = {
      (-b-sqrt(delta))/a,
      (-b+sqrt(delta))/a
    };

    vec3 p[2] = {
      eyeLocalSpace + t[0] * dirLocalSpace,
      eyeLocalSpace + t[1] * dirLocalSpace
    };

    for (int i = 0; i < 2; i++) {
      vec3 point = p[i];
      if (abs(point.y) <= 1.0f) {
        color = vec4(point + vec3(1.0f), 2.0f) / 2.0f;
        return;
      }
    }
  }

  if (abs(abs(exit_point.y) - 1.0f) < 0.00001f) {
    float r = exit_point.x*exit_point.x + exit_point.z*exit_point.z;
    if (r <= 1.0f) {
      color = vec4(0.5f, 0.0f, 0.5f, 1.0f);
      return;
    }
  }
  
  discard;
  return;

  color = vec4(entry_point + vec3(1.0f), 2.0f) / 2.0f;



  return;

  color = frag_color;
	//color *= front_facing;
	color += vec4(1.0f);
	color /= 2.0f;
}