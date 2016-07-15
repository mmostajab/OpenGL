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
#define MAX_FRAGMENTS 60

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

    uint i, j;

    if (fragment_count > 1){

        for (i = 0; i < fragment_count - 1; i++){
            for (j = i + 1; j < fragment_count; j++){
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

	int found = 0;
	int delta = 0;
	int negDepth = 0, posDepth = 0;
	vec4 final_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	for (i = 0; i < fragment_count; i++){
		vec4   frag_color =  unpackUnorm4x8(fragment_list[i].y);
		vec4 meta_data = unpackSnorm4x8(fragment_list[i].w);

		if(meta_data.y >= 0.0f)
			delta = +1;
		else 
			delta = -1;

		if(meta_data.x >= 0.0f){
			posDepth += delta;
		} else {
			negDepth += delta;
		}

		// if(posDepth > 0 && negDepth <= 0){
		//	found = 1;
		//	break;
		//}

		final_color.rgb = final_color.a * (0.3f * frag_color.rgb) + final_color.rgb;
        final_color.a   = 0 + 0.7f * final_color.a;

	}

	color = final_color;
	return;
	
	if(found == 0) {
		discard;
		return;
	}

    color		 = mix(color_background, final_color, final_color.a) ;
    normal_depth = vec4(0.0f);
	gl_FragDepth = fragment_list[fragment_count - 1].z;
}