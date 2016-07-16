#version 450 core

layout (binding = 0, r32ui) 		uniform uimage2D head_pointer_image;
layout (binding = 1, rgba32ui) 		uniform writeonly uimageBuffer list_buffer;
layout (binding = 0, offset = 0) 	uniform atomic_uint list_counter;

layout(location = 0) out vec4 color;

in GS_OUT {
	vec3  normal;
	float sign;
} fs_in;

void main(){
	vec4 frag_color = vec4( (fs_in.normal + vec3(1.0f)) / 2.0f, 1.0f );

	uint index = atomicCounterIncrement(list_counter);
	uint old_head   = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), uint(index));

	float front_facing = +1.0f;
	if(!gl_FrontFacing) front_facing = -1.0f;

	vec4 meta_data = vec4(fs_in.sign, front_facing, 0, 0);

	uvec4 item;
    item.x = old_head;
    item.y = packUnorm4x8(frag_color);
    item.z = floatBitsToUint(gl_FragCoord.z);
    item.w = packSnorm4x8(meta_data);

    imageStore(list_buffer, int(index), item);

    color = frag_color;
}