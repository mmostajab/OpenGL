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

  setNSegs(_nSegs);
  createBuffer();
}

void ArcQuad::createBuffer() {

#ifdef USE_OPENSG
  std::vector<OSG::Pnt3f> vertices;
#else
  std::vector<Vertex> vertices;
#endif


      if (
#ifdef USE_OPENSG
        0
#else
        buffer <= 0
#endif
        || (2 * (nSegs[0] + 2)) > bufferSizeMaxNumSegs[0]) {
        
        
#ifdef USE_OPENSG
          
#else
        if (buffer > 0) 
          glDeleteBuffers(1, &buffer);

        
#endif
        
        bufferSizeMaxNumSegs[0] = bufferSizeMaxNumSegs[1] = 2 * (nSegs[0] + 2);
#ifdef USE_OPENSG  
#else
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSizeMaxNumSegs[0] * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
#endif
      }

      

      Vertex v;

      Vector3Df a[] = { halfArcQuad[0].p0 - halfArcQuad[0].center, halfArcQuad[1].p0 - halfArcQuad[1].center };
      Vector3Df b[] = { halfArcQuad[0].p1 - halfArcQuad[0].center, halfArcQuad[1].p1 - halfArcQuad[1].center };
      float cos_alpha = ArcPrimitiveHelper::angle_between(a[1], b[1]);
      //float epsilon() = 0.01f;
      float alpha = acosf(cos_alpha);

      for (int j = 0; j < nSegs[0] + 1; j++) {
        float t = static_cast<float>(j) / static_cast<float>(nSegs[0]);
        float thetha = t * alpha;

        Vector3Df p[2];

#ifdef USE_SLERP
      p[0] = ArcPrimitiveHelper::slerp(a[0], b[0], thetha, alpha);
      p[1] = ArcPrimitiveHelper::slerp(a[1], b[1], thetha, alpha);
#endif

#ifdef USE_COMPLEX_METHOD
        p[0] = ArcPrimitiveHelper::interpolation_complex(a[0], b[0], thetha, alpha);
        p[1] = ArcPrimitiveHelper::interpolation_complex(a[1], b[1], thetha, alpha);
#endif

        v.position = halfArcQuad[0].center + p[0] ;
#ifdef USE_OPENSG
        vertices.push_back(OSG::Pnt3f(v.position));
#else
        vertices.push_back(v);
#endif

        v.position = halfArcQuad[1].center + p[1] ;
#ifdef USE_OPENSG
        vertices.push_back(OSG::Pnt3f(v.position));
#else
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
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
#endif
      nVertices = static_cast<GLint>(vertices.size());      
  }

void ArcQuad::updateBuffer(Matrix4x4f mvp, unsigned int w, unsigned int h) {

  int new_nSegs = 0;

  for (size_t i = 0; i < halfArcQuad.size(); i++) {
    new_nSegs = static_cast<int>(ArcPrimitiveHelper::calcProjectedCurveLength(mvp, w, h, halfArcQuad[i].p0, halfArcQuad[i].p1, halfArcQuad[i].center) / m_tessScale);
  }

  // if the buffer does not need to change
  if (nSegs[0] == new_nSegs) return;

  std::cout << "Number of segments changed from " << nSegs[0] << " to " << new_nSegs << std::endl;
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
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);

  glDisableVertexAttribArray(0);
#endif

}

void ArcQuad::setNSegs(const int & _nSegs)
{
  nSegs[0] = nSegs[1] = _nSegs;
}