#version 430 core

layout (location = 0) uniform mat4 mvp;
layout (location = 1) uniform vec3 eye;

layout (binding = 0, r32ui) 		  uniform uimage2D                head_pointer_image;
layout (binding = 1, rgba32ui) 		uniform writeonly uimageBuffer  list_buffer;
layout (binding = 0, offset = 0) 	uniform atomic_uint             list_counter;

layout(location = 0) out vec4 color;

in GS_OUT {
  vec3  normalized_pos;
  vec3  center;
  float radius;
  float height;
  float sign;
} fs_in;

void storeFragment(ivec2 frag_coord, vec4 frag_color, float depth, vec4 meta_data) {
  uint index    = atomicCounterIncrement(list_counter);
  uint old_head = imageAtomicExchange(head_pointer_image, frag_coord, uint(index));

  uvec4 item;
  item.x = old_head;
  item.y = packUnorm4x8(frag_color);
  item.z = floatBitsToUint(depth);
  item.w = packSnorm4x8(meta_data);

  imageStore(list_buffer, int(index), item);
}

void main(){
  vec3 normal = normalize(fs_in.normalized_pos);
	vec4 frag_color = vec4( (normal + vec3(1.0f)) / 2.0f, 1.0f );

	float front_facing = +1.0f;
	if(!gl_FrontFacing) front_facing = -1.0f;

  vec4 meta_data = vec4(fs_in.sign, front_facing, 0.0f, 0.0f);

  vec4 eyeLocalSpace = vec4((eye - fs_in.center) / vec3(fs_in.radius, fs_in.height / 2.0f, fs_in.radius), 1.0f);
  vec4 dirLocalSpace = vec4(normalize(fs_in.normalized_pos - eyeLocalSpace.xyz), 0.0f);

  vec3 t_max_cands = (vec3( 1.0f) - eyeLocalSpace.xyz) / dirLocalSpace.xyz;
  vec3 t_min_cands = (vec3(-1.0f) - eyeLocalSpace.xyz) / dirLocalSpace.xyz;

  vec3 t_max = max(t_max_cands, t_min_cands);
  vec3 t_min = min(t_max_cands, t_min_cands);

  float t_entry = max(max(t_min.x, t_min.y), t_min.z);
  float t_exit  = min(min(t_max.x, t_max.y), t_max.z);

  vec3 entry_point = eyeLocalSpace.xyz + t_entry * dirLocalSpace.xyz;
  vec3 exit_point  = eyeLocalSpace.xyz + t_exit  * dirLocalSpace.xyz;
 
  if (abs(abs(entry_point.y) - 1.0f) < 0.0001f) {
    float r = entry_point.x*entry_point.x + entry_point.z*entry_point.z;
    if (r <= 1.0f) {
      frag_color = vec4(0.5f, 0.0f, 0.5f, 1.0f);

      vec4 pointWorldCoord = mvp * (vec4(fs_in.center, 0.0f) + (vec4(entry_point, 1.0f) * vec4(fs_in.radius, fs_in.height / 2.0f, fs_in.radius, 1.0f)));
      float ndc_depth = pointWorldCoord.z / pointWorldCoord.w;
      float depth = ((gl_DepthRange.diff * ndc_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

      // storeFragment(frag_color, gl_FragCoord.z, meta_data);
      storeFragment(ivec2(gl_FragCoord.xy), frag_color, depth, meta_data);
      
      color = frag_color;
    }
  }

  float a = (dirLocalSpace.x * dirLocalSpace.x + dirLocalSpace.z * dirLocalSpace.z);
  float b = /*2.0f **/ (eyeLocalSpace.x * dirLocalSpace.x + eyeLocalSpace.z * dirLocalSpace.z);
  float c = (eyeLocalSpace.x * eyeLocalSpace.x + eyeLocalSpace.z * eyeLocalSpace.z - 1.0f);

  float delta = b*b - a*c;

  if (delta > 0.0f) {
      
    float t[2] = {
      (-b-sqrt(delta))/a,
      (-b+sqrt(delta))/a
    };

    vec4 p[2] = {
      eyeLocalSpace + t[0] * dirLocalSpace,
      eyeLocalSpace + t[1] * dirLocalSpace
    };

    for (int i = 0; i < 2; i++) {
      vec4 point = p[i];
      if (abs(point.y) <= 1.0f) {
        frag_color = (point + vec4(1.0f)) / 2.0f;

        vec4 pointWorldCoord = mvp * ( vec4( fs_in.center, 0.0f ) + ( p[i] * vec4( fs_in.radius, fs_in.height / 2.0f, fs_in.radius, 1.0f ) ) );
        float ndc_depth = pointWorldCoord.z / pointWorldCoord.w;
        float depth = ((gl_DepthRange.diff * ndc_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

        if (i == 0)
          meta_data.y = +1.0f;
        else
          meta_data.y = -1.0f;

        storeFragment(ivec2(gl_FragCoord.xy), frag_color, depth, meta_data);

        color = frag_color;
      }
    }
  }


  if (abs(abs(exit_point.y) - 1.0f) < 0.00001f) {
    float r = exit_point.x*exit_point.x + exit_point.z*exit_point.z;
    if (r <= 1.0f) {
      frag_color    = vec4(0.5f, 0.0f, 0.5f, 1.0f);

      vec4 pointWorldCoord = mvp * (vec4(fs_in.center, 0.0f) + ( vec4(exit_point, 1.0f) * vec4(fs_in.radius, fs_in.height / 2.0f, fs_in.radius, 1.0f)));
      float ndc_depth = pointWorldCoord.z / pointWorldCoord.w;
      float depth = ((gl_DepthRange.diff * ndc_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

      meta_data.y = -1.0f;

      //storeFragment(frag_color, gl_FragCoord.z, meta_data);
      storeFragment(ivec2(gl_FragCoord.xy), frag_color, depth, meta_data);
      
      color = frag_color;
    }
  }
}