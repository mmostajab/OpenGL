#ifndef __UNIFORM_DEFINITIONS_H__
#define __UNIFORM_DEFINITIONS_H__

#include <glm/glm.hpp>

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

#endif