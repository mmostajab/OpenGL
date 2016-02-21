#version 430 core

// Current buffer:
// The per-pixel image containing the head pointers
layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
// Buffer containing linked lists of fragments
layout (binding = 1, rgba32ui) uniform uimageBuffer list_buffer;
layout (binding = 0, offset = 0) uniform atomic_uint list_counter;

// New buffer:
// The per-pixel image containing the head pointers
layout (binding = 2, r32ui) uniform uimage2D new_head_pointer_image;
// Buffer containing linked lists of fragments
layout (binding = 4, rgba32ui) uniform uimageBuffer new_list_buffer;

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
uvec4 new_fragments_list[MAX_FRAGMENTS];

void main(void) {
  uint current_index;
  uint fragment_counter = 0;

  // Copy the linked list in local array
  current_index = imageLoad(head_pointer_image, ivec2(gl_FragCoord).xy).x;
  while (current_index != 0 && fragment_counter < MAX_FRAGMENTS){
        uvec4 fragment = imageLoad(list_buffer, int(current_index));
        fragment_list[fragment_counter] = fragment;
        current_index = fragment.x;
        fragment_counter++;
  }
    
  uint new_fragments_count = 0;
  uint new_fragments_headptr_idx = imageLoad(new_head_pointer_image, ivec2(gl_FragCoord).xy).x;
  current_index = new_fragments_headptr_idx;  
  while (current_index != 0 && new_fragments_count < MAX_FRAGMENTS){
    uvec4 fragment = imageLoad(new_list_buffer, int(current_index));
    new_fragments_list[new_fragments_count] = fragment;
    current_index = fragment.x;
    new_fragments_count++;
  }

  // merging the new fragments into current sorted fragments.
  uint i = 0, j = 0;
  uint new_fragment_idx, current_fragment_idx;

  while(i < new_fragments_count && j < fragment_counter){
    uvec4 new_fragment = new_fragments_list[i];
    uvec4 fragment     = fragment_list[j];
        
    float new_depth = uintBitsToFloat(new_fragment.z);
    float old_depth = uintBitsToFloat(fragment.z);
       
    bool v = new_depth >= old_depth;    
    if(new_depth >= old_depth){ 
      j++;
    } else {
      new_fragment_idx = atomicCounterIncrement( list_counter );
      current_fragment_idx = 0;

      // not the last element in current sorted fragment buffer
      if(j < fragment_counter - 1)
      {
        current_fragment_idx = fragment_list[j+1].x;     
      }
      else
      {
        current_fragment_idx = new_fragments_headptr_idx;
      }
            
      new_fragment.x     = fragment.x;
      fragment.x         = new_fragment_idx;
      imageStore(list_buffer, int(current_fragment_idx), fragment);      
      imageStore(list_buffer, int(new_fragment_idx), new_fragment);
      i++;
    }
  }

  // append the remaining new elements to the fragments list
  uint prev_fragment_idx, link_fragment_idx;
  for(uint i_prime = i; i_prime < new_fragments_count; i_prime++){
    uvec4 new_fragment = new_fragments_list[i_prime];
    new_fragment_idx = atomicCounterIncrement( list_counter );
    if(i_prime > i)
    {
      new_fragment.x = prev_fragment_idx;
      imageStore(list_buffer, int(new_fragment_idx), new_fragment);
    }
    else
    {
      link_fragment_idx = new_fragment_idx;
    }

    prev_fragment_idx = new_fragment_idx;
  }
    
  // If any of the new fragments are appended to the fragment buffer list, head pointer should changed.
  if(i < new_fragments_count){
    uint old_head = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), int(new_fragment_idx));
    
    new_fragments_list[i].x = old_head;
    imageStore(list_buffer, int(link_fragment_idx), new_fragments_list[i]);
  }
}