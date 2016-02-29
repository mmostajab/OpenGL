#version 430 core

// Current buffer:
// The per-pixel image containing the head pointers
layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
// Buffer containing linked lists of fragments
layout (binding = 1, rgba32ui) uniform uimageBuffer list_buffer;
layout (binding = 0, offset = 0) uniform atomic_uint list_counter;

// New buffer:
// The per-pixel image containing the head pointers
layout (binding = 2, r32ui) uniform readonly uimage2D new_head_pointer_image;
// Buffer containing linked lists of fragments
layout (binding = 3, rgba32ui) uniform readonly uimageBuffer new_list_buffer;

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
  uint fragment_headptr_idx = imageLoad(head_pointer_image, ivec2(gl_FragCoord).xy).x;
  current_index = fragment_headptr_idx;
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

  // nothing to merge
  if(fragment_counter == 0 && new_fragments_count == 0) return;

  int i = 0, j = 0;
  uint new_fragment_idx = 0, current_fragment_idx = fragment_headptr_idx;
  if(fragment_counter > 0){
	  // merging the new fragments into current sorted fragments until one of the arrays are over.  
	  while(i < new_fragments_count && j < fragment_counter){
  		uvec4 new_fragment = new_fragments_list[i];
  		uvec4 fragment     = fragment_list[j];
          
  		float new_depth = uintBitsToFloat(new_fragment.z);
  		float old_depth = uintBitsToFloat(fragment.z);
         
  		if(new_depth >= old_depth){ 

  		  // LINKING TWO LINKED LISTS
  		  // if the new fragments should be appended to the previously stored.
  		  if(j == int(fragment_counter) - 1 && i < new_fragments_count){
    			new_fragment_idx = atomicCounterIncrement( list_counter );
    			fragment.x = new_fragment_idx;
    			current_fragment_idx = j > 0 ? fragment_list[j-1].x : fragment_headptr_idx;
    			imageStore(list_buffer, int(current_fragment_idx), fragment);
    			//color = vec4(1.0f,0.0f, 0.0f, 1.0f);//unpackUnorm4x8(new_fragment.y);
  		  }

  		  j++;

  		} else {
  		  new_fragment_idx		= atomicCounterIncrement( list_counter );
  		  current_fragment_idx	= j > 0 ? fragment_list[j-1].x : fragment_headptr_idx;
  		  new_fragment.x		= new_fragment_idx; //fragment.x;
  		  imageStore(list_buffer, int(current_fragment_idx), new_fragment);      
  		  imageStore(list_buffer, int(new_fragment_idx), fragment);
  		  i++;
  		}
	  }
  } else {
    new_fragment_idx = atomicCounterIncrement(list_counter);
	  imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), uint(new_fragment_idx));
  }

  // append the remaining new elements to the fragments list
  uint curr_new_fragment_idx = new_fragment_idx;
  for(; i < int(new_fragments_count)-1; i++){
    uvec4 new_fragment		= new_fragments_list[i];
    uint  next_fragment_idx = atomicCounterIncrement( list_counter );
	  new_fragment.x			= next_fragment_idx;
	  imageStore(list_buffer, int(curr_new_fragment_idx), new_fragment);
    curr_new_fragment_idx		= next_fragment_idx;
  }
    
  // 
  if(i < int(new_fragments_count)){
      uvec4 new_fragment = new_fragments_list[i];
	  new_fragment.x = 0;
	  imageStore(list_buffer, int(curr_new_fragment_idx), new_fragment);    
  }

  color = fragment_counter > 2 ? vec4(0.0f) :  unpackUnorm4x8(fragment_list[2].y);
}