#include "DynTessArcPrimitive.h"

using namespace ArcRep;

DynTessArcPrimitive::DynTessArcPrimitive(DynamicTessellatedPrimitiveType _type) :
#ifndef USE_OPENSG
  buffer(0),
#endif
  m_type(_type), m_tessScale(1.0f), 
  m_dropCullingFactor(5.0f), m_triangulationAccuracy(1.0f),
  m_tessMethod(TESS_METHOD_FIXED_ALPHA),
  //m_tessMethod(TESS_METHOD_CURVE_LENGTH),
  m_disabled(false),
  buffer_size_bytes(0)
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

#else

  AABB aabb;
  aabb.min = glm::vec3(-1.0f);
  aabb.max = glm::vec3( 1.0f);
  setAABB(aabb);

#endif
}

void DynTessArcPrimitive::updateGLBuffer()
{
  size_t required_buffer_size = vertices.size() * sizeof(Vertex);
  buffer_filled_bytes = required_buffer_size;

#ifndef USE_OPENSG
  if (m_glBufferCreationDisabled) return;

  //if (true){
  if (buffer <= 0 || required_buffer_size > buffer_size_bytes) {
    if (buffer > 0) glDeleteBuffers(1, &buffer);
    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, required_buffer_size, vertices.data(), GL_STATIC_DRAW);
    buffer_size_bytes = required_buffer_size;
  }
  else {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, required_buffer_size, vertices.data(), GL_STATIC_DRAW);
  }
#endif
}

void ArcRep::DynTessArcPrimitive::drawBoundingBox()
{
#ifdef USE_OPENSG

#else
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, aabb_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

  glDrawArrays(GL_LINES, 0, 24);

  glDisableVertexAttribArray(0);
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

void ArcRep::DynTessArcPrimitive::setAABB(AABB aabb)
{
  m_aabb = aabb;

#ifndef USE_OPENSG
  std::array<Vertex, 24> vertices;

  glm::vec3 dim = m_aabb.max - m_aabb.min;
  
  vertices[0].position = m_aabb.min;
  vertices[1].position = m_aabb.min + glm::vec3(dim[0], 0.0f, 0.0f);
  vertices[2].position = m_aabb.min;
  vertices[3].position = m_aabb.min + glm::vec3(0.0f, dim[1], 0.0f);
  vertices[4].position = m_aabb.min;
  vertices[5].position = m_aabb.min + glm::vec3(0.0f, 0.0f, dim[2]);

  vertices[6].position = m_aabb.max;
  vertices[7].position = m_aabb.max - glm::vec3(dim[0], 0.0f, 0.0f);
  vertices[8].position = m_aabb.max;
  vertices[9].position = m_aabb.max - glm::vec3(0.0f, dim[1], 0.0f);
  vertices[10].position = m_aabb.max;
  vertices[11].position = m_aabb.max - glm::vec3(0.0f, 0.0f, dim[2]);

  vertices[12].position = m_aabb.max - glm::vec3(dim[0], 0.0f, 0.0f);
  vertices[13].position = m_aabb.min + glm::vec3(0.0f, dim[1], 0.0f);
  vertices[14].position = m_aabb.max - glm::vec3(dim[0], 0.0f, 0.0f);
  vertices[15].position = m_aabb.min + glm::vec3(0.0f, 0.0f, dim[2]);

  vertices[16].position = m_aabb.max - glm::vec3(0.0f, dim[1], 0.0f);
  vertices[17].position = m_aabb.min + glm::vec3(dim[0], 0.0f, 0.0f);
  vertices[18].position = m_aabb.max - glm::vec3(0.0f, dim[1], 0.0f);
  vertices[19].position = m_aabb.min + glm::vec3(0.0f, 0.0f, dim[2]);

  vertices[20].position = m_aabb.max - glm::vec3(0.0f, 0.0f, dim[2]);
  vertices[21].position = m_aabb.min + glm::vec3(dim[0], 0.0f, 0.0f);
  vertices[22].position = m_aabb.max - glm::vec3(0.0f, 0.0f, dim[2]);
  vertices[23].position = m_aabb.min + glm::vec3(0.0f, dim[1], 0.0f);

  glGenBuffers(1, &aabb_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, aabb_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
#endif
}

size_t ArcRep::DynTessArcPrimitive::getFilledMemBytes() const
{
  return buffer_filled_bytes;
}

const std::vector<Vertex>& DynTessArcPrimitive::getTriangleVertices()
{
  triangleVertices.clear();

  if (m_disabled) return triangleVertices;

  triangleVertices = vertices;

  /*switch (bufferDrawPrimType) {
  case GL_TRIANGLE_FAN:
    for (size_t i = 1; i < vertices.size() - 1; i++) {
      triangleVertices.push_back(vertices[0]);
      triangleVertices.push_back(vertices[i + 0]);
      triangleVertices.push_back(vertices[i + 1]);
    }
    break;
  case GL_TRIANGLE_STRIP:
    for (size_t i = 0; i < vertices.size() - 2; i++) {
      if (i % 2 == 0) {
        triangleVertices.push_back(vertices[i + 0]);
        triangleVertices.push_back(vertices[i + 1]);
        triangleVertices.push_back(vertices[i + 2]);
      }
      else {
        triangleVertices.push_back(vertices[i + 1]);
        triangleVertices.push_back(vertices[i + 0]);
        triangleVertices.push_back(vertices[i + 2]);
      }
    }
    break;
  case GL_TRIANGLES:
    triangleVertices = vertices;
  }*/

  return triangleVertices;
}

#ifndef USE_OPENSG
void ArcRep::DynTessArcPrimitive::disableGLBufferCreation()
{
  m_glBufferCreationDisabled = true;
}

void ArcRep::DynTessArcPrimitive::enableGLBufferCreation()
{
  m_glBufferCreationDisabled = false;
}
#endif

void ArcRep::DynTessArcPrimitive::disableUpdateEveryFrame()
{
  m_updateEveryFrame = false;
}

void ArcRep::DynTessArcPrimitive::enableUpdateEveryFrame()
{
  m_updateEveryFrame = true;
}

#ifdef USE_OPENSG

const std::vector<Vertex>& ArcRep::DynTessArcPrimitive::getVertices() const {
//const std::vector<Vertex>&  DynTessArcPrimitive::getVertices() const {
  return vertices;
}

const std::vector<uint32_t>& DynTessArcPrimitive::getLengths()  const {
  return lengths;
}

const std::vector<OSG::UInt8>&  DynTessArcPrimitive::getTypes()    const {
  return types;
}

#endif