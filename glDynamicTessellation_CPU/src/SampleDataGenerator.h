#ifndef __SAMPLE_DATA_GENERATOR_H__
#define __SAMPLE_DATA_GENERATOR_H__

#include "ArcSegment.h"
#include "ArcTriangle.h"
#include "ArcQuad.h"

#include <glm/glm.hpp>

static void addLogo(
  glm::vec3 p, glm::float32 angle, glm::float32 scale,
  std::vector<ArcSegment>&   arcSegs,
  std::vector<ArcTriangle>&  arcTriangles,
  std::vector<ArcQuad>&      arcQuads
) {
  ArcTriangle arc[4];
  
  arc[0].p1      = glm::vec3(1.0f, 0.0f, 0.0f);
  arc[0].p2      = glm::vec3(0.0f, 1.0f, 0.0f);
  arc[0].p3      = glm::vec3(1.0f, 1.0f, 0.0f);
  arc[0].center  = glm::vec3(0.0f, 0.0f, 0.0f);
  arcTriangles.push_back(arc[0]);

  arc[1].p1      = glm::vec3( 0.0f, 1.0f, 0.0f);
  arc[1].p2      = glm::vec3(-1.0f, 0.0f, 0.0f);
  arc[1].p3      = glm::vec3(-1.0f, 1.0f, 0.0f);
  arc[1].center  = glm::vec3( 0.0f, 0.0f, 0.0f);
  arcTriangles.push_back(arc[1]);

  arc[2].p1      = glm::vec3(-1.0f,  0.0f, 0.0f);
  arc[2].p2      = glm::vec3( 0.0f, -1.0f, 0.0f);
  arc[2].p3      = glm::vec3(-1.0f, -1.0f, 0.0f);
  arc[2].center  = glm::vec3( 0.0f, 0.0f, 0.0f);
  arcTriangles.push_back(arc[2]);

  arc[3].p1      = glm::vec3( 0.0f, -1.0f, 0.0f);
  arc[3].p2      = glm::vec3( 1.0f,  0.0f, 0.0f);
  arc[3].p3      = glm::vec3( 1.0f, -1.0f, 0.0f);
  arc[3].center  = glm::vec3( 0.0f, 0.0f, 0.0f);
  arcTriangles.push_back(arc[3]);
}

static void addC(
  std::vector<ArcSegment>& arcSegments, std::vector<ArcTriangle> arcTriangles, std::vector<ArcQuad>& arcQuads
) {
  ArcSegment arcSegment0, arcSegment1;

  arcSegment0.p1 = glm::vec3(1.4f, 1.4f, 0.0f);
  arcSegment0.p2 = glm::vec3(0.7f, 0.7f, 0.0f);
  arcSegment0.center = glm::vec3(0.0f, 2.0f, 0.0f);
  arcSegment0.createBuffer(5);
  arcSegments.push_back(arcSegment0);

  arcSegment1.p1 = glm::vec3(1.4f, -1.4f, 0.0f);
  arcSegment1.p2 = glm::vec3(0.7f, -0.7f, 0.0f);
  arcSegment1.center = glm::vec3(0.0f, -2.0f, 0.0f);
  arcSegment1.createBuffer(5);
  arcSegments.push_back(arcSegment1);

  ArcQuad arcQuad0, arcQuad1, arcQuad2, arcQuad3;
  arcQuad0.halfArcQuad[0].p0 = glm::vec3(0.0f, 2.0f, 0);
  arcQuad0.halfArcQuad[0].p1 = glm::vec3(1.4f, 1.4f, 0);
  arcQuad0.halfArcQuad[0].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad0.halfArcQuad[1].p0 = glm::vec3(0.0f, 1.0f, 0);
  arcQuad0.halfArcQuad[1].p1 = glm::vec3(0.7f, 0.7f, 0);
  arcQuad0.halfArcQuad[1].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = glm::vec3(0.0f, -2.0f, 0);
  arcQuad1.halfArcQuad[0].p1 = glm::vec3(0.0f, 2.0f, 0);
  arcQuad1.halfArcQuad[0].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad1.halfArcQuad[1].p0 = glm::vec3(0.0f, -1.0f, 0);
  arcQuad1.halfArcQuad[1].p1 = glm::vec3(0.0f, 1.0f, 0);
  arcQuad1.halfArcQuad[1].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);

  arcQuad2.halfArcQuad[0].p0 = glm::vec3(1.4f, -1.4f, 0);
  arcQuad2.halfArcQuad[0].p1 = glm::vec3(0.0f, -2.0f, 0);
  arcQuad2.halfArcQuad[0].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad2.halfArcQuad[1].p0 = glm::vec3(0.7f, -0.7f, 0);
  arcQuad2.halfArcQuad[1].p1 = glm::vec3(0.0f, -1.0f, 0);
  arcQuad2.halfArcQuad[1].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad2.createBuffer(5);
  arcQuads.push_back(arcQuad2);
}

static void addS(
  std::vector<ArcSegment>& arcSegments, std::vector<ArcTriangle> arcTriangles, std::vector<ArcQuad>& arcQuads
) {
  ArcSegment arcSegment0, arcSegment1;

  arcSegment0.p1 = glm::vec3(2.0f, 0.0f, 0.0f);
  arcSegment0.p2 = glm::vec3(1.0f, 0.0f, 0.0f);
  arcSegment0.center = glm::vec3(2.5f, 1.0f, 0.0f);
  arcSegment0.createBuffer(5);
  arcSegments.push_back(arcSegment0);

  arcSegment1.p1 = glm::vec3(-2.0f, -3.0f, 0.0f);
  arcSegment1.p2 = glm::vec3(-1.0f, -3.0f, 0.0f);
  arcSegment1.center = glm::vec3(-2.5f, -5.0f, 0.0f);
  arcSegment1.createBuffer(5);
  arcSegments.push_back(arcSegment1);

  ArcQuad arcQuad0, arcQuad1, arcQuad2, arcQuad3;
  arcQuad0.halfArcQuad[0].p0 = glm::vec3(-2.0f, 0.0f, 0);
  arcQuad0.halfArcQuad[0].p1 = glm::vec3(2.0f, 0.0f, 0);
  arcQuad0.halfArcQuad[0].center = glm::vec3(0.0f, -0.0f, 0);

  arcQuad0.halfArcQuad[1].p0 = glm::vec3(-1.0f, 0.0f, 0);
  arcQuad0.halfArcQuad[1].p1 = glm::vec3(1.0f, 0.0f, 0);
  arcQuad0.halfArcQuad[1].center = glm::vec3(0.0f, -0.0f, 0);

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = glm::vec3(0.0f, -2.0f, 0);
  arcQuad1.halfArcQuad[0].p1 = glm::vec3(-2.0f, 0.0f, 0);
  arcQuad1.halfArcQuad[0].center = glm::vec3(0.0f, -0.0f, 0);

  arcQuad1.halfArcQuad[1].p0 = glm::vec3(0.0f, -1.0f, 0);
  arcQuad1.halfArcQuad[1].p1 = glm::vec3(-1.0f, 0.0f, 0);
  arcQuad1.halfArcQuad[1].center = glm::vec3(0.0f, -0.0f, 0);

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);

  arcQuad2.halfArcQuad[0].p0 = glm::vec3(0.0f, -2.0f, 0);
  arcQuad2.halfArcQuad[0].p1 = glm::vec3(0.0f, -4.0f, 0);
  arcQuad2.halfArcQuad[0].center = glm::vec3(0.0f, -3.0f, 0);

  arcQuad2.halfArcQuad[1].p0 = glm::vec3(0.0f, -1.0f, 0);
  arcQuad2.halfArcQuad[1].p1 = glm::vec3(0.0f, -5.0f, 0);
  arcQuad2.halfArcQuad[1].center = glm::vec3(0.0f, -3.0f, 0);

  arcQuad2.createBuffer(5);
  arcQuads.push_back(arcQuad2);

  arcQuad3.halfArcQuad[0].p0 = glm::vec3(0.0f, -4.0f, 0);
  arcQuad3.halfArcQuad[0].p1 = glm::vec3(-1.0f, -3.0f, 0);
  arcQuad3.halfArcQuad[0].center = glm::vec3(0.0f, -3.0f, 0);

  arcQuad3.halfArcQuad[1].p0 = glm::vec3(0.0f, -5.0f, 0);
  arcQuad3.halfArcQuad[1].p1 = glm::vec3(-2.0f, -3.0f, 0);
  arcQuad3.halfArcQuad[1].center = glm::vec3(0.0f, -3.0f, 0);

  arcQuad3.createBuffer(5);
  arcQuads.push_back(arcQuad3);
}

static void addT(
  std::vector<ArcSegment>& arcSegments, std::vector<ArcTriangle> arcTriangles, std::vector<ArcQuad>& arcQuads
) {
  ArcQuad arcQuad0, arcQuad1;
  arcQuad0.halfArcQuad[0].p0 = glm::vec3(-1.4f, 1.4f, 0);
  arcQuad0.halfArcQuad[0].p1 = glm::vec3(+1.4f, 1.4f, 0);
  arcQuad0.halfArcQuad[0].center = glm::vec3(0.0f, -0.0f, 0);

  arcQuad0.halfArcQuad[1].p0 = glm::vec3(-1.0f, 1.0f, 0);
  arcQuad0.halfArcQuad[1].p1 = glm::vec3(+1.0f, 1.0f, 0);
  arcQuad0.halfArcQuad[1].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = glm::vec3(0.0f, -2.0f, 0);
  arcQuad1.halfArcQuad[0].p1 = glm::vec3(-2.0f, 0.0f, 0);
  arcQuad1.halfArcQuad[0].center = glm::vec3(0.0f, -0.0f, 0);

  arcQuad1.halfArcQuad[1].p0 = glm::vec3(0.0f, -1.0f, 0);
  arcQuad1.halfArcQuad[1].p1 = glm::vec3(-1.0f, 0.0f, 0);
  arcQuad1.halfArcQuad[1].center = glm::vec3(0.0f, -0.0f, 0);

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);
}

static void addM(
  std::vector<ArcSegment>& arcSegments, std::vector<ArcTriangle> arcTriangles, std::vector<ArcQuad>& arcQuads
) {
  ArcQuad arcQuad0, arcQuad1, arcQuad2;
  arcQuad0.halfArcQuad[1].p0 = glm::vec3(+1.4f, 1.4f, 0);
  arcQuad0.halfArcQuad[1].p1 = glm::vec3(-1.4f, 1.4f, 0);
  arcQuad0.halfArcQuad[1].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad0.halfArcQuad[0].p0 = glm::vec3(+1.4f, 1.0f, 0);
  arcQuad0.halfArcQuad[0].p1 = glm::vec3(-1.4f, 1.0f, 0);
  arcQuad0.halfArcQuad[0].center = glm::vec3(0.0f, 0.0f, 0);

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = glm::vec3(+1.8f, +1.4f, 0);
  arcQuad1.halfArcQuad[0].p1 = glm::vec3(+1.4f, +1.4f, 0);
  arcQuad1.halfArcQuad[0].center = glm::vec3(+1.6f, 1.6f, 0);

  arcQuad1.halfArcQuad[1].p0 = glm::vec3(+1.8f, -2.0f, 0);
  arcQuad1.halfArcQuad[1].p1 = glm::vec3(+1.4f, -2.0f, 0);
  arcQuad1.halfArcQuad[1].center = glm::vec3(+1.6f, -1.8f, 0);

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);

  arcQuad2.halfArcQuad[0].p0 = glm::vec3(-1.4f, +1.4f, 0);
  arcQuad2.halfArcQuad[0].p1 = glm::vec3(-1.8f, +1.4f, 0);
  arcQuad2.halfArcQuad[0].center = glm::vec3(-1.6f, 1.6f, 0);

  arcQuad2.halfArcQuad[1].p0 = glm::vec3(-1.4f, -2.0f, 0);
  arcQuad2.halfArcQuad[1].p1 = glm::vec3(-1.8f, -2.0f, 0);
  arcQuad2.halfArcQuad[1].center = glm::vec3(-1.6f, -1.8f, 0);

  arcQuad2.createBuffer(5);
  arcQuads.push_back(arcQuad2);
}

static void addF(
  std::vector<ArcSegment>& arcSegments, std::vector<ArcTriangle> arcTriangles, std::vector<ArcQuad>& arcQuads
) {
  ArcQuad arcQuad0, arcQuad1, arcQuad2;
  arcQuad0.halfArcQuad[0].p0 = glm::vec3(-2.0f, 2.0f, 0);
  arcQuad0.halfArcQuad[0].p1 = glm::vec3(+2.0f, 2.0f, 0);
  arcQuad0.halfArcQuad[0].center = glm::vec3(0.0f, 2.3f, 0);

  arcQuad0.halfArcQuad[1].p0 = glm::vec3(-2.0f, 1.4f, 0);
  arcQuad0.halfArcQuad[1].p1 = glm::vec3(+2.0f, 1.4f, 0);
  arcQuad0.halfArcQuad[1].center = glm::vec3(0.0f, 29.5f, 0);

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = glm::vec3(+1.0f, -0.5f, 0);
  arcQuad1.halfArcQuad[0].p1 = glm::vec3(-2.0f, -0.5f, 0);
  arcQuad1.halfArcQuad[0].center = glm::vec3(-0.5f, -0.7f, 0);

  arcQuad1.halfArcQuad[1].p0 = glm::vec3(+1.0f, -1.0f, 0);
  arcQuad1.halfArcQuad[1].p1 = glm::vec3(-2.0f, -1.0f, 0);
  arcQuad1.halfArcQuad[1].center = glm::vec3(-0.5f, -1.3f, 0);

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);
  
  arcQuad2.halfArcQuad[0].p0 = glm::vec3(-2.4f, +2.0f, 0);
  arcQuad2.halfArcQuad[0].p1 = glm::vec3(-2.0f, +2.0f, 0);
  arcQuad2.halfArcQuad[0].center = glm::vec3(-2.2f, 3.0f, 0);

  arcQuad2.halfArcQuad[1].p0 = glm::vec3(-2.4f, -4.0f, 0);
  arcQuad2.halfArcQuad[1].p1 = glm::vec3(-2.0f, -4.0f, 0);
  arcQuad2.halfArcQuad[1].center = glm::vec3(-2.2f, -5.0f, 0);

  arcQuad2.createBuffer(5);
  arcQuads.push_back(arcQuad2);
}

static void addCST(
  glm::vec3 p, glm::float32 angle, glm::float32 scale,
  std::vector<ArcSegment>&   arcSegs, 
  std::vector<ArcTriangle>&  arcTriangles, 
  std::vector<ArcQuad>&      arcQuads) {



}

static void sampleDataGenerator(std::vector<ArcSegment>& arcSegs, std::vector<ArcTriangle> arcTriangles, std::vector<ArcQuad>& arcQuads) {

}

#endif // __SAMPLE_DATA_GENERATOR_H__