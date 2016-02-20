#version 430 core

// Current buffer:
// The per-pixel image containing the head pointers
layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
// Buffer containing linked lists of fragments
layout (binding = 1, rgba32ui) uniform uimageBuffer list_buffer;

// New buffer:
// The per-pixel image containing the head pointers
layout (binding = 2, r32ui) uniform uimage2D head_pointer_image;
// Buffer containing linked lists of fragments
layout (binding = 3, rgba32ui) uniform uimageBuffer list_buffer;

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
uvec4 new_fragment_list[MAX_FRAGS];


    
    
    
    
    

void main(void) {
    uint current_index;
    uint fragment_count = 0;

    // Copy the linked list in local array
    uint current_index = imageLoad(head_pointer_image, ivec2(gl_FragCoord).xy).x;
	while (current_index != 0 && fragment_count < MAX_FRAGMENTS){
        uvec4 fragment = imageLoad(list_buffer, int(current_index));
        fragment_list[fragment_count] = fragment;
        current_index = fragment.x;
        fragment_count++;
    }
    
    uint new_fragments_count = 0;
    uint headptr_idx = imageLoad(new_headptr_image, ivec2(gl_FragCoord).xy).x;
    current_idx = headptr_idx;
    while (current_idx != 0 && new_fragments_count < MAX_FRAGS){
        uvec4 frag = imageLoad(new_frag_buffer, int(curr_idx));
        new_fragments_list[new_fragments_count] = frag;
        curr_idx = frag.x;
        new_fragments_count++;
    }

    // Sort the new_frag_list with bubble sort.
    
    // merging the new fragments into current sorted fragments.
    uint i, j;
    while(i < new_frag_count && j < frag_count){
        uvec4 new_fragment = new_frag_list[i];
        uvec4 fragment     = tmp_frag_list[j];
        
        float new_depth = uintBitsToFloat(new_fragment.z);
        float depth     = uintBitsToFloat(fragment.z);
        
        if(new_depth >= depth) 
            j++;
        else
        {
            int curr_fragment_idx;
            uint new_frag_idx = atomicCounterIncrement(frag_counter);
            
            // not the last element in current sorted fragment buffer
            if(j < frag_count - 1)
            {
                curr_fragment_idx = tmp_frag_list[j+1].x;     
            }
            else
            {
                curr_fragment_idx = headptr_idx;
            }
            
            new_fragment.x     = fragment.x;
            fragment.x         = new_frag_idx;
            imageStore(frag_buffer, int(curr_frag_idx), fragment);      
            imageStore(frag_buffer, int(new_frag_idx), new_fragment);
            i++;
        }
    }

    // append the remaining new elements to the fragments list
    uint new_frag_idx, prev_frag_idx, link_frag_idx;
    for(int i_prime = i; i_prime < new_frag_count; i_prime++){
        uvec4 new_fragment = new_frag_list[i_prime];
        uint new_frag_idx = atomicCounterIncrement(frag_counter);
        if(i_prime > i)
        {
            new_fragment.x = prev_frag_count;
            imageStore(frag_buffer, int(new_frag_idx), new_fragment);
        }
        else
            link_frag_idx = new_frag_idx;
    }
    
    // If any of the new fragments are appended to the fragment buffer list, head pointer should changed.
    if(i < new_frag_count){
        int old_head = imageAtomicExchange(headptr_image, ivec2(gl_FragCoord.xy), uint(new_frag_idx));
        new_frag_list[i].x = old_head;
        imageStore(frag_buffer, int(new_frag_idx), new_frag_list[i]);
    }
}