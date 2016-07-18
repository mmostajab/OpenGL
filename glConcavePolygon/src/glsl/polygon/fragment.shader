#version 450 core

layout (binding = 0, r32ui) 		uniform uimage2D head_pointer_image;
layout (binding = 1, rgba32ui) 		uniform writeonly uimageBuffer list_buffer;
layout (binding = 0, offset = 0) 	uniform atomic_uint list_counter;

layout(location = 0) out vec4 color;

in VS_OUT {

	vec3 normal;
	float sign;

} fs_in;

void main(){
	vec4 frag_color = vec4( (fs_in.normal + vec3(1.0f)) / 2.0f, 1.0f );
  color = frag_color;
}