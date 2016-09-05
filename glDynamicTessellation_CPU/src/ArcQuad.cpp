#include "ArcQuad.h"

// STD
#include <iostream>

#include "ArcPrimitiveHelper.h"

ArcQuad::ArcQuad(Vector3Df _p1, Vector3Df _p2, Vector3Df _center0, Vector3Df _p3, Vector3Df _p4, Vector3Df _center1, int _nSegs): 
  DynTessArcPrimitive(DYN_TESS_ARC_QUAD)
{
  halfArcQuad[0].p0 = _p1;
  halfArcQuad[0].p1 = _p2;
  halfArcQuad[0].center = _center0;

  halfArcQuad[1].p0 = _p3;
  halfArcQuad[1].p1 = _p4;
  halfArcQuad[1].center = _center1;

#ifdef USE_OPENSG
  transform = OSG::Node::create();
	OSG::TransformPtr transform_core = OSG::Transform::create();
	OSG::Matrix4f matrix;
	//matrix.identity();
	//matrix.setTranslate(translation);
	//transform_core->setMatrix(matrix);
	OSG::beginEditCP(transform);
	transform->setCore(transform_core);
	OSG::endEditCP(transform);

	OSG::NodePtr geoNode = OSG::Node::create();
	OSG::GeometryPtr geoCore = OSG::Geometry::create();
	OSG::beginEditCP(geoCore);
	geoNode->setCore(geoCore);
	OSG::endEditCP(geoCore);

	OSG::beginEditCP(transform);
	//transform->addChild(geoNode);
	OSG::endEditCP(transform);
#endif

  setNSegs(std::array<int32_t, 2>{_nSegs, _nSegs});
  createBuffer();
}

void ArcQuad::createBuffer() {

#ifdef USE_OPENSG
  std::vector<OSG::Pnt3f> vertices;
#else
  std::vector<Vertex> vertices;
#endif

  std::vector<Vector3Df> curve_points[2];
  for (int i = 0; i < 2; i++){
    Vector3Df a     = halfArcQuad[i].p0 - halfArcQuad[i].center;
    Vector3Df b     = halfArcQuad[i].p1 - halfArcQuad[i].center;
    float cos_alpha = ArcPrimitiveHelper::angle_between(a, b);
    float alpha     = acosf(cos_alpha);

    for (int j = 0; j < nSegs[0] + 1; j++) {
      float t = static_cast<float>(j) / static_cast<float>(nSegs[0]);
      float thetha = t * alpha;

      Vector3Df p;

#ifdef USE_SLERP
      p = ArcPrimitiveHelper::slerp(a, b, thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
      p = ArcPrimitiveHelper::interpolation_complex(a, b, thetha, alpha);
#endif
      curve_points[i].push_back(p + halfArcQuad[i].center);
    };
  }
    

  Vertex v;
  size_t i = 0, j = 0;
  while (i+1 < curve_points[0].size() && j+1 < curve_points[1].size()) {

    Vertex v0, v1;
    v0.position = curve_points[0][i];
    v1.position = curve_points[1][j];

    float length_left  = UnifiedMath::length(curve_points[0][i + 1] - curve_points[1][j]);
    float length_right = UnifiedMath::length(curve_points[0][i] - curve_points[1][j + 1]);
    if (length_left <= length_right) {
      v.position = curve_points[0][i + 1];
      i++;
    } else {
      v.position = curve_points[1][j + 1];
      j++;
    }
#ifdef USE_OPENSG
    vertices.push_back(OSG::Pnt3f(v0.position));
    vertices.push_back(OSG::Pnt3f(v1.position));
    vertices.push_back(OSG::Pnt3f(v.position));
#else
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v);
#endif
  }

  for (; i+1 < curve_points[0].size(); i++) {
    Vertex v0, v1;
    v0.position = curve_points[0][i];
    v1.position = curve_points[1][j];
    v.position = curve_points[0][i + 1];

#ifdef USE_OPENSG
    vertices.push_back(OSG::Pnt3f(v0.position));
    vertices.push_back(OSG::Pnt3f(v1.position));
    vertices.push_back(OSG::Pnt3f(v.position));
#else
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v);
#endif
  }

  for (; j + 1 < curve_points[1].size(); j++) {
    Vertex v0, v1;
    v0.position = curve_points[0][i];
    v1.position = curve_points[1][j];
    v.position = curve_points[1][j + 1];
#ifdef USE_OPENSG
    vertices.push_back(OSG::Pnt3f(v0.position));
    vertices.push_back(OSG::Pnt3f(v1.position));
    vertices.push_back(OSG::Pnt3f(v.position));
#else
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v);
#endif
  }

#ifdef USE_OPENSG
  std::vector<OSG::UInt32> lengths;
	std::vector<OSG::UInt8>  types;

  lengths.push_back(static_cast<OSG::UInt32>(vertices.size()));
  types.push_back(GL_TRIANGLE_STRIP);

  createDrawArraysNode(transform, vertices, lengths, types);  
#else

  if (buffer > 0)
    glDeleteBuffers(1, &buffer);
  glCreateBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
#endif
      nVertices = static_cast<GLint>(vertices.size());      
  }

void ArcQuad::updateBuffer(Matrix4x4f mvp, unsigned int w, unsigned int h) {

  std::array<int32_t, 2> new_nSegs = { 0, 0 };

  for (size_t i = 0; i < halfArcQuad.size(); i++) {
    new_nSegs[i] = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, halfArcQuad[i].p0, halfArcQuad[i].p1, halfArcQuad[i].center) / m_tessScale);
  }

  // if the buffer does not need to change
  if (nSegs[0] == new_nSegs[0] && nSegs[1] == new_nSegs[1]) return;

  std::cout << "Number of segments changed from " << nSegs[0] << " to " << new_nSegs[0] << std::endl;
  std::cout << "Number of segments changed from " << nSegs[1] << " to " << new_nSegs[1] << std::endl;
  setNSegs(new_nSegs);
  createBuffer();
}

void ArcQuad::draw() {

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef USE_OPENSG

#else
  glEnableVertexAttribArray(0);
    
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const char*)0);
  glDrawArrays(GL_TRIANGLES, 0, nVertices);

  glDisableVertexAttribArray(0);
#endif

}

void ArcQuad::setNSegs(const std::array<int32_t, 2> & _nSegs)
{
  nSegs[0] = _nSegs[0];
  nSegs[1] = _nSegs[1];
}