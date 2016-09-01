#include "DynTessArcPrimitive.h"

DynTessArcPrimitive::DynTessArcPrimitive(DynamicTessellatedPrimitiveType _type): m_type(_type), m_tessScale(1.0f)
{
}

float DynTessArcPrimitive::getTessScale() const 
{
  return m_tessScale;
}

void DynTessArcPrimitive::setTessScale(float _tessScale)
{    
  m_tessScale = _tessScale;    
}

void DynTessArcPrimitive::multiplyTessFactor(float multiplier)
{
  m_tessScale *= multiplier;
}
