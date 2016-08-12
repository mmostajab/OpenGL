#ifndef __GEOM_STATE_H__
#define __GEOM_STATE_H__

// GLM
#include <glm/glm.hpp>

//
// Simple geometry state data structure
//
struct GeomState
{
  GeomState()
    : material_index(0)
  {}

  glm::int32  material_index;
};

//
// helper to calculate the correct buffer insert positions on std140
//
std::size_t align_offset(std::size_t base_alignment, std::size_t base_offset)
{
  return base_alignment * ((base_alignment + base_offset - 1) / base_alignment);
}

#endif // __GEOM_STATE_H__