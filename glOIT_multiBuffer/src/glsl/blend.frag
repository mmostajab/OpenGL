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

void main(void) {
  uint current_index;
  uint fragment_count = 0;

  current_index = imageLoad(head_pointer_image, ivec2(gl_FragCoord).xy).x;

	while (current_index != 0 && fragment_count < MAX_FRAGMENTS){
        uvec4 fragment = imageLoad(list_buffer, int(current_index));
        fragment_list[fragment_count] = fragment;
        current_index = fragment.x;
        fragment_count++;
    }

    vec4 final_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 frag_color = vec4(1, 0, 0, 0);
    for (int i = 0; i < fragment_count; i++){
        frag_color = unpackUnorm4x8(fragment_list[i].y);
		    vec4 frag_specularity = unpackUnorm4x8(fragment_list[i].w);
        final_color.rgb = final_color.a * (frag_color.a * frag_color.rgb) + final_color.rgb;
        final_color.a   = 0.0f + (1 - frag_color.a) * final_color.a;
        
    }

  color		 = final_color.a * color_background + final_color;
  normal_depth = vec4(0.0f);
	gl_FragDepth = uintBitsToFloat(fragment_list[fragment_count - 1].z);
}