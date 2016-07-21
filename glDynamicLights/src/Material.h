#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "UniformBuffer.h"

// STD
#include <vector>

// GLM
#include <glm/glm.hpp>

// OpenGL
#include <GL/GL.h>

//
// Simple material data structure
//
struct Material
{
    Material()
    : ambient (0.f, 0.f, 0.f)
    , diffuse (0.f, 0.f, 0.f)
    , specular(0.f, 0.f, 0.f)
    , emissive(0.f, 0.f, 0.f)
    , opacity(1.f)
    , shininess(100.f)
    {}

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;

    glm::float32  opacity;
    glm::float32  shininess;
};

std::vector<Material> initialize_materials(std::size_t num)           // helper to create materials
{
  std::vector<Material> materials;

  for (std::size_t i = 0; i < num; ++i)
  {
    Material m;

    m.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    m.diffuse = glm::vec3(1.0f, 0.3f, 0.2f);
    m.specular = glm::vec3(0.9f, 0.9f, 0.9f);
    m.emissive = glm::vec3(0.0f, 0.0f, 0.0f);
    m.opacity = 1.f;
    m.shininess = 80.f;

    materials.push_back(m);
  }

  return materials;
}

const std::size_t num_materials = 100;                                  // any number of materials
std::vector<Material> materials = initialize_materials(num_materials);  // the material database

//
// the material uniform buffer object
// see comment *) at the top of the file
//
std::size_t calc_material_database_buffer_size(const std::vector<Material>& materials)
{
  std::size_t ao = 0; // aligned offset
  std::size_t bo = 0; // base offset

  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4); // OSG::Color3f ambient;
  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4); // OSG::Color3f diffuse;
  ao = align_offset(16, bo); bo = ao + sizeof(OSG::Color4f); // OSG::Color3f specular;
  ao = align_offset(16, bo); bo = ao + sizeof(OSG::Color4f); // OSG::Color3f emissive;
  ao = align_offset(4, bo); bo = ao + sizeof(OSG::Real32);  // OSG::Real32  opacity;
  ao = align_offset(4, bo); bo = ao + sizeof(OSG::Real32);  // OSG::Real32  shininess;
  ao = align_offset(16, bo); bo = ao;                        // padding

  ao *= materials.size();     bo = ao;                        // array
  ao = align_offset(16, bo); bo = ao;                        // padding

  return ao;
}

std::vector<OSG::UInt8> create_material_database_buffer(const VecMaterialsT& materials)
{
  std::size_t size = calc_material_database_buffer_size(materials);

  std::vector<OSG::UInt8> buffer(size);

  std::size_t ao = 0; // aligned offset
  std::size_t bo = 0; // base offset

  for (std::size_t i = 0; i < materials.size(); ++i)
  {
    ao = align_offset(16, bo);
    memcpy(&buffer[0] + ao, &materials[i].ambient[0], sizeof(OSG::Color3f));
    bo = ao + sizeof(OSG::Color4f);

    ao = align_offset(16, bo);
    memcpy(&buffer[0] + ao, &materials[i].diffuse[0], sizeof(OSG::Color3f));
    bo = ao + sizeof(OSG::Color4f);

    ao = align_offset(16, bo);
    memcpy(&buffer[0] + ao, &materials[i].specular[0], sizeof(OSG::Color3f));
    bo = ao + sizeof(OSG::Color4f);

    ao = align_offset(16, bo);
    memcpy(&buffer[0] + ao, &materials[i].emissive[0], sizeof(OSG::Color3f));
    bo = ao + sizeof(OSG::Color4f);

    ao = align_offset(4, bo);
    *(reinterpret_cast<OSG::Real32*>(&buffer[0] + ao)) = materials[i].opacity;
    bo = ao + sizeof(OSG::Real32);

    ao = align_offset(4, bo);
    *(reinterpret_cast<OSG::Real32*>(&buffer[0] + ao)) = materials[i].shininess;
    bo = ao + sizeof(OSG::Real32);

    ao = align_offset(16, bo); bo = ao;    // padding
  }

  return buffer;
}

//OSG::UniformBufferObjStd140ChunkTransitPtr create_material_database_state(const VecMaterialsT& materials)
GLuint create_material_database_state(const std::vector<Material>& materials)
{
  OSG::UniformBufferObjStd140ChunkRefPtr ubo = OSG::UniformBufferObjStd140Chunk::create();

  std::vector<OSG::UInt8> buffer = create_material_database_buffer(materials);

  ubo->editMFBuffer()->setValues(buffer);
  ubo->setUsage(GL_STATIC_DRAW);

  return OSG::UniformBufferObjStd140ChunkTransitPtr(ubo);
}

void update_material_database_state(OSG::UniformBufferObjStd140Chunk* ubo, const VecMaterialsT& materials)
{
  if (ubo) {
    std::vector<OSG::UInt8> buffer = create_material_database_buffer(materials);
    ubo->editMFBuffer()->setValues(buffer);
  }
}

//
// the geomertry uniform buffer object
//
std::size_t calc_geometry_material_buffer_size()
{
  std::size_t ao = 0; // aligned offset
  std::size_t bo = 0; // base offset

  ao = align_offset(4, bo); bo = ao + sizeof(OSG::UInt32);  // OSG::UInt32  material_index;
  ao = align_offset(16, bo); bo = ao;                        // padding

  return ao;
}

#endif // __MATERIAL_H__