#include "DynTessArcPrimitive.h"

using namespace ArcRep;

DynTessArcPrimitive::DynTessArcPrimitive(DynamicTessellatedPrimitiveType _type) :
  buffer(0), m_type(_type), m_tessScale(1.0f), m_dropCullingFactor(5.0f), m_triangulationAccuracy(1.0f),
  m_tessMethod(TESS_METHOD_FIXED_ALPHA)
  //m_tessMethod(TESS_METHOD_CURVE_LENGTH)
{
#ifdef USE_OPENSG

  /*
  switchNode = OSG::Node::create();
  OSG::SwitchPtr switchNode_core = OSG::Switch::create();
  switchNode->setCore(switchNode_core);
  

  transform = OSG::Node::create();
	OSG::TransformPtr transform_core = OSG::Transform::create();
	OSG::Matrix4f matrix;
	matrix.identity();
  matrix.setTranslate(OSG::Pnt3f(0.0f, 0.0f, 0.0f));
	transform_core->setMatrix(matrix);
	
  OSG::beginEditCP(transform);
	transform->setCore(transform_core);
	OSG::endEditCP(transform);

  OSG::beginEditCP(switchNode);
  switchNode->addChild(transform);
  OSG::endEditCP(switchNode);
  */

#endif
}

//float DynTessArcPrimitive::getTessScale() const 
//{
//  return m_tessScale;
//}
//
//void DynTessArcPrimitive::setTessScale(float _tessScale)
//{    
//  m_tessScale = _tessScale;    
//}

void DynTessArcPrimitive::multiplyTessFactor(float multiplier)
{
  m_tessScale *= multiplier;
}

void ArcRep::DynTessArcPrimitive::setDropCullingFactor(float factor)
{
  if (factor > 0)
    m_dropCullingFactor = factor;
  else
    m_dropCullingFactor = 1.0f;
}

void ArcRep::DynTessArcPrimitive::setTrianulationAccuracyFactor(float factor)
{
  if (factor > 0)
    m_triangulationAccuracy = factor;
  else
    m_triangulationAccuracy = 1.0f;
}

#ifdef USE_OPENSG

const std::vector<OSG::Pnt3f>&  DynTessArcPrimitive::getVertices() const {
  return vertices;
}

const std::vector<OSG::UInt32>& DynTessArcPrimitive::getLengths()  const {
  return lengths;
}

const std::vector<OSG::UInt8>&  DynTessArcPrimitive::getTypes()    const {
  return types;
}

#endif