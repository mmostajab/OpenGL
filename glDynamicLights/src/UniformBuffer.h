#ifndef __UNIFORM_BUFFER_H__
#define __UNIFORM_BUFFER_H__

//
// helper to calculate the correct buffer insert positions on std140
//
std::size_t align_offset(std::size_t base_alignment, std::size_t base_offset)
{
  return base_alignment * ((base_alignment + base_offset - 1) / base_alignment);
}


#endif