#version 430 core

// The per-pixel image containing the head pointers
layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
// Buffer containing linked lists of fragments
layout (binding = 1, rgba32ui) uniform uimageBuffer list_buffer;

layout(std140, binding = 3) uniform GeneralBlock {
    float color_multiplier;
	vec4 color_background;
} ;

// This is the output color
layout (location = 0) out vec4 color;
layout (location = 1) out vec4 normal_depth;

layout( location = 10 ) uniform int obj_order = 0;

// This is the maximum number of overlapping fragments allowed
#define MAX_FRAGMENTS 40

// Temporary array used for sorting fragments
uvec4 fragment_list[MAX_FRAGMENTS];
uint  index_list[MAX_FRAGMENTS];

void main(void) {
    uint current_index;
    int fragment_count = 0;

    current_index = imageLoad(head_pointer_image, ivec2(gl_FragCoord).xy).x;
	  while (current_index != 0 && fragment_count < MAX_FRAGMENTS){
        uvec4 fragment = imageLoad(list_buffer, int(current_index));
        fragment_list[fragment_count] = fragment;
        index_list[fragment_count]    = current_index;
        current_index = fragment.x;
        fragment_count++;
    }

    if (fragment_count > 1){

        for (uint i = 0; i < fragment_count - 1; i++){
            for (uint j = i + 1; j < fragment_count; j++){
                uvec4 fragment1 = fragment_list[i];
                uvec4 fragment2 = fragment_list[j];

                float depth1 = uintBitsToFloat(fragment1.z);
                float depth2 = uintBitsToFloat(fragment2.z);

                if (depth1 > depth2){
                    fragment_list[i] = fragment2;
                    fragment_list[j] = fragment1;
                }
            }
        }

    }

    // Write-back the sorted fragments.
    int i = 0;
    for(; i < fragment_count-1; i++){
      fragment_list[i].x = index_list[i+1];
      imageStore(list_buffer, int(index_list[i]), fragment_list[i]);
    }
    if(fragment_count > 0){
      fragment_list[i].x = 0;
      imageStore(list_buffer, int(index_list[i]), fragment_list[i]);
    }
}