#include "ArcTriangle.h"

#include "ArcPrimitiveHelper.h"

// STD
#include <iostream>

ArcTriangle::ArcTriangle(
    Vector3Df _p1,
    Vector3Df _p2,
    Vector3Df _p3,
    Vector3Df _center/*double _alpha = glm::pi<double>() / 2.0f*/,
    int _nSegs): DynTessArcPrimitive(DYN_TESS_ARC_TRIANGLE) {

    p1 = _p1;
    p2 = _p2;
    p3 = _p3;
    center = _center;
    //alpha = _alpha;

#ifdef USE_OPENSG
  transform = OSG::Node::create();
	OSG::TransformPtr transform_core = OSG::Transform::create();
	OSG::Matrix4f matrix;
	matrix.identity();
  matrix.setTranslate(OSG::Pnt3f(0.0f, 0.0f, 0.0f));
	transform_core->setMatrix(matrix);
	OSG::beginEditCP(transform);
	transform->setCore(transform_core);
	OSG::endEditCP(transform);

	OSG::NodePtr geoNode = OSG::Node::create();
	OSG::GeometryPtr geoCore = OSG::Geometry::create();
	OSG::beginEditCP(geoCore);
	geoNode->setCore(geoCore);
	OSG::endEditCP(geoCore);

	OSG::beginEditCP(transform);
	transform->addChild(geoNode);
	OSG::endEditCP(transform);
#endif

    setNSegs(_nSegs);
    createBuffer();
  }

  void ArcTriangle::createBuffer() {

    
#ifdef USE_OPENSG

#else
    if (buffer > 0) {
      glDeleteBuffers(1, &buffer);
      buffer = 0;
    }
#endif
    

#ifdef USE_OPENSG
  std::vector<OSG::Pnt3f> vertices;
#else
  std::vector<Vertex>     vertices;
#endif

    Vertex v;
    v.position = p3;

    
#ifdef USE_OPENSG
  vertices.push_back(v.position);
#else
  vertices.push_back(v);
#endif

    

    Vector3Df a = p1 - center;
    Vector3Df b = p2 - center;
    float cos_alpha = ArcPrimitiveHelper::angle_between(a, b);
    float alpha = acosf(cos_alpha);

    for (int i = 0; i < nSegs + 1; i++) {
      float t = static_cast<float>(i) / static_cast<float>(nSegs);
      float thetha = t * alpha;

      Vector3Df p;
#ifdef USE_SLERP
      p = ArcPrimitiveHelper::slerp(a, b, thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
      p = ArcPrimitiveHelper::interpolation_complex(a, b, -thetha, -alpha);
#endif

      v.position = p + center;
#ifdef USE_OPENSG
      vertices.push_back(v.position);
#else
      vertices.push_back(v);
#endif
    }

#ifdef USE_OPENSG
  std::vector<OSG::UInt32> lengths;
	std::vector<OSG::UInt8>  types;

  lengths.push_back(static_cast<OSG::UInt32>(vertices.size()));
	types.push_back(GL_TRIANGLE_FAN);

  createDrawArraysNode(transform, vertices, lengths, types);  
#else
    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
#endif

    nVertices = static_cast<GLint>(vertices.size());
  }

  void ArcTriangle::updateBuffer(Matrix4x4f mvp, unsigned int w, unsigned int h) {
    
    int new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, p1, p2, center) / m_tessScale);

    if (new_nSegs == nSegs) return;

    std::cout << "Number of segments changed from " << nSegs << " to " << new_nSegs << std::endl;
    setNSegs(new_nSegs);
    createBuffer();

  }

  void ArcTriangle::draw() {

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#ifdef USE_OPENSG

#else

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);

    glDisableVertexAttribArray(0);
#endif

  }

void ArcTriangle::setNSegs(const int& _nSegs)
{
  nSegs = _nSegs;
}
