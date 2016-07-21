#ifndef __UNIFORM_DEFINITIONS_H__
#define __UNIFORM_DEFINITIONS_H__

// STD
#include <vector>
#include <malloc.h>
#include <stdio.h>

// GLM
#include <glm/glm.hpp>

#include "UniformBuffer.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
};

struct Light
{
  enum Type
  {
    DIRECTIONAL_LIGHT = 0,
    POINT_LIGHT       = 1,
    SPOT_LIGHT        = 2,
    NO_LIGHT
  };

  Light()
    : type(0, 0, 0)
    , position(0.f, 0.f, 0.f)
    , spot_direction(0.f, 1.f, 0.f)
    , Ia(1.f, 1.f, 1.f)
    , Id(1.f, 1.f, 1.f)
    , Is(1.f, 1.f, 1.f)
    , attenuation(1.f, 0.f, 0.f)
    , spot_cos_cutoff(cosf(45.f))
    , spot_exponent(1.f)
  {}

  bool isEnabled()
  {
    if (type[0] == 1 || type[1] == 1 || type[2] == 1)
      return true;
    else
      return false;
  }

  static glm::ivec3 getLightType(Type e)
  {
    switch (e) {
    case DIRECTIONAL_LIGHT: return glm::ivec3(1.f, 0.f, 0.f);
    case POINT_LIGHT:       return glm::ivec3(0.f, 1.f, 0.f);
    case SPOT_LIGHT:        return glm::ivec3(0.f, 0.f, 1.f);
    case NO_LIGHT:          return glm::ivec3(0.f, 0.f, 0.f);
    default:                return glm::ivec3(0.f, 0.f, 0.f);
    }
  }

  static Light create_light(Type e)
  {
    Light l;
    l.type = getLightType(e);

    switch (e) {
    case DIRECTIONAL_LIGHT:
      l.spot_direction = glm::vec3(1.f, 0.f, 0.f);
      break;
    case POINT_LIGHT: 
      l.position = glm::vec3(0.f, 0.2f, 0.f);
      break;
    case SPOT_LIGHT: 
      l.position        = glm::vec3(0.f, 0.2f, 0.f); 
      l.spot_direction  = glm::vec3(0.f, 0.f, 0.f) - l.position;
      break;
    default:
      break;
    }
    return l;
  }

  glm::ivec3  type;                // (dir,point,spot) value is either 1 or 0 and maximal one component can be 1
  glm::vec3   position;             // in world space
  glm::vec3   spot_direction;       // in world space, also used for dir of directional lights (see shader code)
  glm::vec3   Ia;                   // ambient  max. Intensity
  glm::vec3   Id;                   // diffuse  max. Intensity
  glm::vec3   Is;                   // specular max. Intensity

  glm::vec3     attenuation;        // (constant, linear, quadratic) with constant >= 1 and linear,quadratic >= 0
  glm::float32  spot_cos_cutoff;    // cosine cut of angle
  glm::float32  spot_exponent;      // [0-128]
};



static std::vector<Light> initialize_lights(const std::vector<Light::Type>& types)         // helper to create lights
{
  std::vector<Light> lights;

  for (std::size_t i = 0; i < types.size(); ++i)
    lights.push_back(Light::create_light(types[i]));

  return lights;
}

//
// the light uniform buffer object 
// see comment *) at the top of the file
//
std::size_t calc_light_buffer_size(const std::vector<Light>& lights)
{
  std::size_t ao = 0; // aligned offset
  std::size_t bo = 0; // base offset
  
  ao = align_offset(16, bo); bo = ao + sizeof(glm::ivec4);   // OSG::Vec3i   type;
  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4);    // OSG::Pnt3f   position;
  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4);    // OSG::Vec3f   spot_direction;
  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4);    // OSG::Color3f Ia;
  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4);    // OSG::Color3f Id;
  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4);    // OSG::Color3f Is;
  ao = align_offset(16, bo); bo = ao + sizeof(glm::vec4);    // OSG::Vec3f   attenuation;
  ao = align_offset(4,  bo); bo = ao + sizeof(glm::float32); // OSG::Real32  spot_cos_cutoff;
  ao = align_offset(4,  bo); bo = ao + sizeof(glm::float32); // OSG::Real32  spot_exponent;
  ao = align_offset(16, bo); bo = ao;                        // padding

  ao *= lights.size();       bo = ao;                        // array
  ao = align_offset(16, bo); bo = ao;                        // padding

  return ao;
}

std::vector<OSG::UInt8> create_light_buffer(const std::vector<Light>& lights, glm::mat4 view_mat)
{
  std::size_t size = calc_light_buffer_size(lights);

  std::vector<glm::uint8> buffer(size);

  std::size_t ao = 0; // aligned offset
  std::size_t bo = 0; // base offset

  for (std::size_t i = 0; i < lights.size(); ++i)
  {
    glm::vec3 position_es = glm::mat3(view_mat) * lights[i].position;             // transform_to_eye_space(lights[i].position, mgr);
    glm::vec3 spot_direction_es = glm::mat3(view_mat) * lights[i].spot_direction; // view_mattransform_to_eye_space(lights[i].spot_direction, mgr);

    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &lights[i].type[0], sizeof(OSG::Vec3i));
    memcpy(&buffer[0] + ao, &lights[i].type[0], sizeof(glm::ivec3));
    //bo = ao + sizeof(OSG::Vec4i);
    bo = ao + sizeof(glm::ivec4);

    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &position_es[0], sizeof(OSG::Pnt3f));
    //bo = ao + sizeof(OSG::Pnt4f);
    memcpy(&buffer[0] + ao, &position_es[0], sizeof(glm::vec3));
    bo = ao + sizeof(glm::vec4);

    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &spot_direction_es[0], sizeof(OSG::Vec3f));
    //bo = ao + sizeof(OSG::Vec4f);
    memcpy(&buffer[0] + ao, &spot_direction_es[0], sizeof(glm::vec3));
    bo = ao + sizeof(glm::vec4);

    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &lights[i].Ia[0], sizeof(OSG::Color3f));
    //bo = ao + sizeof(OSG::Color4f);
    memcpy(&buffer[0] + ao, &lights[i].Ia[0], sizeof(glm::vec3));
    bo = ao + sizeof(glm::vec4);

    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &lights[i].Id[0], sizeof(OSG::Color3f));
    //bo = ao + sizeof(OSG::Color4f);
    memcpy(&buffer[0] + ao, &lights[i].Id[0], sizeof(glm::vec3));
    bo = ao + sizeof(glm::vec4);

    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &lights[i].Is[0], sizeof(OSG::Color3f));
    //bo = ao + sizeof(OSG::Color4f);
    memcpy(&buffer[0] + ao, &lights[i].Is[0], sizeof(glm::vec3));
    bo = ao + sizeof(glm::vec4);

    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &lights[i].attenuation[0], sizeof(OSG::Vec3f));
    //bo = ao + sizeof(OSG::Vec4f);
    memcpy(&buffer[0] + ao, &lights[i].attenuation[0], sizeof(glm::vec3));
    bo = ao + sizeof(glm::vec4);

    ao = align_offset(4, bo);
    //*(reinterpret_cast<OSG::Real32*>(&buffer[0] + ao)) = lights[i].spot_cos_cutoff;
    //bo = ao + sizeof(OSG::Real32);
    *(reinterpret_cast<glm::float32*>(&buffer[0] + ao)) = lights[i].spot_cos_cutoff;
    bo = ao + sizeof(glm::float32);

    ao = align_offset(4, bo);
    //*(reinterpret_cast<OSG::Real32*>(&buffer[0] + ao)) = lights[i].spot_cos_cutoff;
    //bo = ao + sizeof(OSG::Real32);
    ao = align_offset(16, bo);
    //memcpy(&buffer[0] + ao, &lights[i].attenuation[0], sizeof(OSG::Vec3f));
    //bo = ao + sizeof(OSG::Vec4f);
    memcpy(&buffer[0] + ao, &lights[i].spot_direction[0], sizeof(glm::vec3));
    bo = ao + sizeof(glm::vec4);

    ao = align_offset(4, bo);
    //*(reinterpret_cast<OSG::Real32*>(&buffer[0] + ao)) = lights[i].spot_exponent;
    //bo = ao + sizeof(OSG::Real32);
    *(reinterpret_cast<glm::float32*>(&buffer[0] + ao)) = lights[i].spot_exponent;
    bo = ao + sizeof(glm::float32);

    ao = align_offset(16, bo); bo = ao;    // padding
  }

  return buffer;
}

GLuint create_light_state(const std::vector<Light>& lights)
{
  GLuint lights_uniform_buffer;

  glGenBuffers(1, &lights_uniform_buffer);
  glBindBuffer(GL_UNIFORM_BUFFER, lights_uniform_buffer);
  glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

  //std::vector<OSG::UInt8> buffer = create_light_buffer(lights);
  std::vector<glm::uint8> buffer = create_light_buffer(lights);

  glGenBuffers(1, &lights_uniform_buffer);
  glBindBuffer(GL_UNIFORM_BUFFER, lights_uniform_buffer);
  glBufferData(GL_UNIFORM_BUFFER, buffer.size(), NULL, GL_DYNAMIC_DRAW);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_uniform_buffer);
  glm::uint8* lightsPtr = (glm::uint8*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, buffer.size(), GL_MAP_WRITE_BIT);
  memcpy(lightsPtr, buffer.data(), buffer.size());
  glUnmapBuffer(GL_UNIFORM_BUFFER);

  return lights_uniform_buffer;

  //ubo->editMFBuffer()->setValues(buffer);
  //ubo->setUsage(GL_DYNAMIC_DRAW);
  //return OSG::UniformBufferObjStd140ChunkTransitPtr(ubo);
}

//void update_light_state(OSG::UniformBufferObjStd140Chunk* ubo, const VecLightsT& lights)
void update_light_state(GLuint lights_uniform_buffer, const std::vector<Light>& lights)
{
  if (lights_uniform_buffer > 0) {
    //std::vector<OSG::UInt8> buffer = create_light_buffer(lights);
    std::vector<glm::uint8> buffer = create_light_buffer(lights);
    //ubo->editMFBuffer()->setValues(buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_uniform_buffer);
    glm::uint8* lightsPtr = (glm::uint8*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, buffer.size(), GL_MAP_WRITE_BIT);
    memcpy(lightsPtr, buffer.data(), buffer.size());
    glUnmapBuffer(GL_UNIFORM_BUFFER);
  }
}

std::vector<OSG::UInt8> create_geometry_material_buffer(const GeomState& geom_state)
{
  std::size_t size = calc_geometry_material_buffer_size();

  std::vector<OSG::UInt8> buffer(size);

  std::size_t ao = 0; // aligned offset
  std::size_t bo = 0; // base offset

  ao = align_offset(4, bo);
  *(reinterpret_cast<OSG::UInt32*>(&buffer[0] + ao)) = geom_state.material_index;
  bo = ao + sizeof(OSG::UInt32);

  return buffer;
}

OSG::UniformBufferObjStd140ChunkTransitPtr create_geometry_material_state(const GeomState& geom_state)
{
  OSG::UniformBufferObjStd140ChunkRefPtr ubo = OSG::UniformBufferObjStd140Chunk::create();

  std::vector<OSG::UInt8> buffer = create_geometry_material_buffer(geom_state);

  ubo->editMFBuffer()->setValues(buffer);
  ubo->setUsage(GL_DYNAMIC_DRAW);

  return OSG::UniformBufferObjStd140ChunkTransitPtr(ubo);
}

void update_geometry_material_state(OSG::UniformBufferObjStd140Chunk* ubo, const GeomState& geom_state)
{
  if (ubo) {
    std::vector<OSG::UInt8> buffer = create_geometry_material_buffer(geom_state);
    ubo->editMFBuffer()->setValues(buffer);
  }
}

#endif