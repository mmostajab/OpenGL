#ifndef __SAMPLE_DATA_GENERATOR_H__
#define __SAMPLE_DATA_GENERATOR_H__

#include "ArcSegment.h"
#include "ArcTriangle.h"
#include "ArcQuad.h"

#include <glm/glm.hpp>

#define TRANSFORM(mat, p) \
  glm::vec3(mat * glm::vec4(p, 1.0f))

static void addLogo(
  glm::vec3 p, glm::float32 angle, glm::float32 scale,
  std::vector<ArcSegment>&   arcSegs,
  std::vector<ArcTriangle>&  arcTriangles,
  std::vector<ArcQuad>&      arcQuads
) {

  glm::mat4 mat =
    glm::translate<float>(p) *
    glm::rotate<float>(angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
    glm::scale<float>(glm::vec3(scale)) *

    glm::translate<float>(glm::vec3(0.0f, 0.5f, 0.0f)) *

    glm::scale<float>(glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f));

  ArcTriangle arc[4];
  
  arc[0].p1      = TRANSFORM(mat, glm::vec3(1.0f, 0.0f, 0.0f));
  arc[0].p2      = TRANSFORM(mat, glm::vec3(0.0f, 1.0f, 0.0f));
  arc[0].p3      = TRANSFORM(mat, glm::vec3(1.0f, 1.0f, 0.0f));
  arc[0].center  = TRANSFORM(mat, glm::vec3(1.0f, 1.0f, 0.0f));
  arcTriangles.push_back(arc[0]);

  arc[1].p1      = TRANSFORM(mat, glm::vec3( 0.0f, 1.0f, 0.0f));
  arc[1].p2      = TRANSFORM(mat, glm::vec3(-1.0f, 0.0f, 0.0f));
  arc[1].p3      = TRANSFORM(mat, glm::vec3(-1.0f, 1.0f, 0.0f));
  arc[1].center  = TRANSFORM(mat, glm::vec3( 0.0f, 0.0f, 0.0f));
  arcTriangles.push_back(arc[1]);

  arc[2].p1      = TRANSFORM(mat, glm::vec3(-1.0f,  0.0f, 0.0f));
  arc[2].p2      = TRANSFORM(mat, glm::vec3( 0.0f, -1.0f, 0.0f));
  arc[2].p3      = TRANSFORM(mat, glm::vec3(-1.0f, -1.0f, 0.0f));
  arc[2].center  = TRANSFORM(mat, glm::vec3( 0.0f, 0.0f, 0.0f));
  arcTriangles.push_back(arc[2]);

  arc[3].p1      = TRANSFORM(mat, glm::vec3( 0.0f, -1.0f, 0.0f));
  arc[3].p2      = TRANSFORM(mat, glm::vec3( 1.0f,  0.0f, 0.0f));
  arc[3].p3      = TRANSFORM(mat, glm::vec3( 1.0f, -1.0f, 0.0f));
  arc[3].center  = TRANSFORM(mat, glm::vec3( 0.0f, 0.0f, 0.0f));
  arcTriangles.push_back(arc[3]);
}

static void addC(
  glm::vec3 p, glm::float32 angle, glm::float32 scale,
  std::vector<ArcSegment>& arcSegments, 
  std::vector<ArcTriangle>& arcTriangles, 
  std::vector<ArcQuad>& arcQuads
) {

  // BBOX = MIN[-2.0f, -2.0f] MAX[1.4f, 2.0f]

  glm::mat4 mat = 
    glm::translate<float>(p) * 
    glm::rotate<float>(angle, glm::vec3(0.0f, 0.0f, 1.0f)) * 
    glm::scale<float>(glm::vec3(scale)) * 

    glm::translate<float>(glm::vec3(0.0f, 0.5f, 0.0f)) *

    glm::scale<float>(glm::vec3(1.0f / 3.4f, 1.0f / 4.0f, 1.0f)) *
    glm::translate<float>(glm::vec3(+0.3f, 0.0f, 0.0f));

  ArcSegment arcSegment0, arcSegment1;

  arcSegment0.p1 = TRANSFORM(mat, glm::vec3(1.4f, 1.4f, 0.0f));
  arcSegment0.p2 = TRANSFORM(mat, glm::vec3(0.7f, 0.7f, 0.0f));
  arcSegment0.center = TRANSFORM(mat, glm::vec3(1.05f, 1.05f, 0.0f));
  arcSegment0.createBuffer(5);
  arcSegments.push_back(arcSegment0);

  arcSegment1.p1 = TRANSFORM(mat, glm::vec3(0.7f, -0.7f, 0.0f));
  arcSegment1.p2 = TRANSFORM(mat, glm::vec3(1.4f, -1.4f, 0.0f));
  arcSegment1.center = TRANSFORM(mat, glm::vec3(1.05f, -1.05f, 0.0f));
  arcSegment1.createBuffer(5);
  arcSegments.push_back(arcSegment1);

  ArcQuad arcQuad0, arcQuad1, arcQuad2, arcQuad3;
  arcQuad0.halfArcQuad[0].p0 = TRANSFORM(mat, glm::vec3(0.0f, 2.0f, 0));
  arcQuad0.halfArcQuad[0].p1 = TRANSFORM(mat, glm::vec3(1.4f, 1.4f, 0));
  arcQuad0.halfArcQuad[0].center = TRANSFORM(mat, glm::vec3(0.0f, 0.0f, 0));

  arcQuad0.halfArcQuad[1].p0 = TRANSFORM(mat, glm::vec3(0.0f, 1.0f, 0));
  arcQuad0.halfArcQuad[1].p1 = TRANSFORM(mat, glm::vec3(0.7f, 0.7f, 0));
  arcQuad0.halfArcQuad[1].center = TRANSFORM(mat, glm::vec3(0.0f, 0.0f, 0));

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = TRANSFORM(mat, glm::vec3(0.0f, -2.0f, 0));
  arcQuad1.halfArcQuad[0].p1 = TRANSFORM(mat, glm::vec3(0.0f, 2.0f, 0));
  arcQuad1.halfArcQuad[0].center = TRANSFORM(mat, glm::vec3(0.0f, 0.0f, 0));

  arcQuad1.halfArcQuad[1].p0 = TRANSFORM(mat, glm::vec3(0.0f, -1.0f, 0));
  arcQuad1.halfArcQuad[1].p1 = TRANSFORM(mat, glm::vec3(0.0f, 1.0f, 0));
  arcQuad1.halfArcQuad[1].center = TRANSFORM(mat, glm::vec3(0.0f, 0.0f, 0));

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);

  arcQuad2.halfArcQuad[0].p0 = TRANSFORM(mat, glm::vec3(1.4f, -1.4f, 0));
  arcQuad2.halfArcQuad[0].p1 = TRANSFORM(mat, glm::vec3(0.0f, -2.0f, 0));
  arcQuad2.halfArcQuad[0].center = TRANSFORM(mat, glm::vec3(0.0f, 0.0f, 0));

  arcQuad2.halfArcQuad[1].p0 = TRANSFORM(mat, glm::vec3(0.7f, -0.7f, 0));
  arcQuad2.halfArcQuad[1].p1 = TRANSFORM(mat, glm::vec3(0.0f, -1.0f, 0));
  arcQuad2.halfArcQuad[1].center = TRANSFORM(mat, glm::vec3(0.0f, 0.0f, 0));

  arcQuad2.createBuffer(5);
  arcQuads.push_back(arcQuad2);
}

static void addS(
  glm::vec3 p, glm::float32 angle, glm::float32 scale,
  std::vector<ArcSegment>& arcSegments, 
  std::vector<ArcTriangle>& arcTriangles, 
  std::vector<ArcQuad>& arcQuads
) {

  // BBOX = MIN[-2.0f, -5.0f] MAX[2.0f, +2.0f]

  glm::mat4 mat =
    glm::translate<float>(p) *
    glm::rotate<float>(angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
    glm::scale<float>(glm::vec3(scale)) *
    
    glm::translate<float>(glm::vec3(0.0f, 0.5f, 0.0f))
    ;
  
  float s_width = 0.2f;

  float outer_radius = (0.5f + s_width / 2.0f) / 2.0f;
  float iner_radius = ((0.5f - s_width) - s_width / 2.0f) / 2.0f;

  glm::vec3 outer_center(0.0f, 0.5f - outer_radius, 0.0f);
  glm::vec3 iner_center(0.0f, 0.5f - s_width - iner_radius, 0.0f);

  ArcSegment arcSegment0, arcSegment1;

  arcSegment0.p1 = TRANSFORM(mat, glm::vec3(outer_radius, outer_center.y, 0));
  arcSegment0.p2 = TRANSFORM(mat, glm::vec3(iner_radius, iner_center.y, 0));
  arcSegment0.center = TRANSFORM(mat, (0.5f * glm::vec3(outer_radius + iner_radius, (iner_center.y + outer_center.y), 0.0f)));
  arcSegment0.createBuffer(5);
  arcSegments.push_back(arcSegment0);

  arcSegment1.p1 = TRANSFORM(mat, glm::vec3(-outer_radius, -outer_center.y, 0));
  arcSegment1.p2 = TRANSFORM(mat, glm::vec3(-iner_radius, -iner_center.y, 0));
  arcSegment1.center = TRANSFORM(mat, (-0.5f * glm::vec3(outer_radius + iner_radius, (iner_center.y + outer_center.y), 0.0f)));
  arcSegment1.createBuffer(5);
  arcSegments.push_back(arcSegment1);



  ArcQuad arcQuad0, arcQuad1, arcQuad2, arcQuad3;
  arcQuad0.halfArcQuad[0].p0      = TRANSFORM(mat, glm::vec3(-outer_radius, outer_center.y, 0));
  arcQuad0.halfArcQuad[0].p1      = TRANSFORM(mat, glm::vec3( outer_radius, outer_center.y, 0));
  arcQuad0.halfArcQuad[0].center  = TRANSFORM(mat, outer_center);

  arcQuad0.halfArcQuad[1].p0      = TRANSFORM(mat, glm::vec3(-iner_radius, iner_center.y, 0));
  arcQuad0.halfArcQuad[1].p1      = TRANSFORM(mat, glm::vec3( iner_radius, iner_center.y, 0));
  arcQuad0.halfArcQuad[1].center  = TRANSFORM(mat, iner_center);

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = TRANSFORM(mat, glm::vec3(0.0f, outer_center.y - outer_radius, 0));
  arcQuad1.halfArcQuad[0].p1      = TRANSFORM(mat, glm::vec3(-outer_radius, outer_center.y, 0));
  arcQuad1.halfArcQuad[0].center  = TRANSFORM(mat, outer_center );

  arcQuad1.halfArcQuad[1].p0 = TRANSFORM(mat, glm::vec3(0.0f, iner_center.y - iner_radius, 0));
  arcQuad1.halfArcQuad[1].p1      = TRANSFORM(mat, glm::vec3(-iner_radius,  iner_center.y,  0));
  arcQuad1.halfArcQuad[1].center  = TRANSFORM(mat, iner_center);

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);

  arcQuad2.halfArcQuad[0].p0      = TRANSFORM(mat, glm::vec3(0.0f, outer_center.y - outer_radius, 0));
  arcQuad2.halfArcQuad[0].p1      = TRANSFORM(mat, glm::vec3(0.0f, -iner_center.y - iner_radius, 0));
  arcQuad2.halfArcQuad[0].center  = TRANSFORM(mat, -iner_center);

  arcQuad2.halfArcQuad[1].p0      = TRANSFORM(mat, glm::vec3(0.0f, iner_center.y - iner_radius, 0));
  arcQuad2.halfArcQuad[1].p1      = TRANSFORM(mat, glm::vec3(0.0f, -outer_center.y - outer_radius, 0));
  arcQuad2.halfArcQuad[1].center  = TRANSFORM(mat, -outer_center);

  arcQuad2.createBuffer(5);
  arcQuads.push_back(arcQuad2);

  arcQuad3.halfArcQuad[0].p1      = TRANSFORM(mat, glm::vec3(-iner_radius, -iner_center.y, 0));
  arcQuad3.halfArcQuad[0].p0      = TRANSFORM(mat, glm::vec3(0.0f, -iner_center.y - iner_radius, 0));
  arcQuad3.halfArcQuad[0].center  = TRANSFORM(mat, -iner_center);

  arcQuad3.halfArcQuad[1].p1      = TRANSFORM(mat, glm::vec3(-outer_radius, -outer_center.y, 0));
  arcQuad3.halfArcQuad[1].p0      = TRANSFORM(mat, glm::vec3(0.0f, -outer_center.y - outer_radius, 0));
  arcQuad3.halfArcQuad[1].center  = TRANSFORM(mat, -outer_center);

  arcQuad3.createBuffer(5);
  arcQuads.push_back(arcQuad3);
}

static void addT(
  glm::vec3 p, glm::float32 angle, glm::float32 scale,
  std::vector<ArcSegment>& arcSegments, 
  std::vector<ArcTriangle>& arcTriangles, 
  std::vector<ArcQuad>& arcQuads
) {

  // BBOX = MIN[-2.0f, 0.0f] MAX[2.0f, 5.6f]

  glm::mat4 rot = 
    glm::translate<float>(p) * 
    glm::rotate<float>(angle, glm::vec3(0.0f, 0.0f, 1.0f)) * 
    glm::scale<float>(glm::vec3(scale)) *

    glm::translate<float>(glm::vec3(.0f, 0.5f, 0.0f)) *

    glm::scale<float>(glm::vec3(1.0f / 4.0f, 1.0f / 5.6f, 1.0f)) *
    glm::translate<float>(glm::vec3(0.0f, -2.8, 0.0f));

  // (-2.0f, -3.0f) => (2.0f, 1.2f) 

  ArcTriangle arcTriangle0, arcTriangle1;
  arcTriangle0.p1 = TRANSFORM(rot, glm::vec3(-1.0f, 4.0f, 0));
  arcTriangle0.p2 = TRANSFORM(rot, glm::vec3(-2.0f, 4.1f, 0));
  arcTriangle0.p3 = TRANSFORM(rot, glm::vec3(-1.8f, 4.2f, 0));
  arcTriangle0.center = TRANSFORM(rot, glm::vec3(-1.4f, 4.2f, 0));
  arcTriangles.push_back(arcTriangle0);

  arcTriangle1.p1 = TRANSFORM(rot, glm::vec3(1.0f, 4.0f, 0));
  arcTriangle1.p2 = TRANSFORM(rot, glm::vec3(2.0f, 4.1f, 0));
  arcTriangle1.p3 = TRANSFORM(rot, glm::vec3(1.8f, 4.2f, 0));
  arcTriangle1.center = TRANSFORM(rot, glm::vec3(1.4f, 4.2f, 0));
  arcTriangles.push_back(arcTriangle1);

  ArcQuad arcQuad0, arcQuad1;
  arcQuad0.halfArcQuad[0].p0 = TRANSFORM(rot, glm::vec3(-1.8f, 4.2f, 0));
  arcQuad0.halfArcQuad[0].p1 = TRANSFORM(rot, glm::vec3(+1.8f, 4.2f, 0));
  arcQuad0.halfArcQuad[0].p1 = TRANSFORM(rot, glm::vec3(+1.8f, 4.2f, 0));
  arcQuad0.halfArcQuad[0].center = TRANSFORM(rot, glm::vec3(0.0f, 3.0f, 0));

  arcQuad0.halfArcQuad[1].p0 = TRANSFORM(rot, glm::vec3(-1.0f, 4.0f, 0));
  arcQuad0.halfArcQuad[1].p1 = TRANSFORM(rot, glm::vec3(+1.0f, 4.0f, 0));
  arcQuad0.halfArcQuad[1].center = TRANSFORM(rot, glm::vec3(0.0f, 3.0f, 0));

  arcQuad0.createBuffer(5);
  arcQuads.push_back(arcQuad0);

  arcQuad1.halfArcQuad[0].p0 = TRANSFORM(rot, glm::vec3(-0.4f, 4.0f, 0));
  arcQuad1.halfArcQuad[0].p1 = TRANSFORM(rot, glm::vec3( 0.4f, 4.0f, 0));
  arcQuad1.halfArcQuad[0].center = TRANSFORM(rot, glm::vec3(0.0f, 3.0f, 0));

  arcQuad1.halfArcQuad[1].p0 = TRANSFORM(rot, glm::vec3(-0.2f, 0.0f, 0));
  arcQuad1.halfArcQuad[1].p1 = TRANSFORM(rot, glm::vec3(0.2f, 0.0f, 0));
  arcQuad1.halfArcQuad[1].center = TRANSFORM(rot, glm::vec3(0.0f, -1.0f, 0));

  arcQuad1.createBuffer(5);
  arcQuads.push_back(arcQuad1);
}

static void addM(
  std::vector<ArcSegment>& arcSegments, 
  std::vector<ArcTriangle>& arcTriangles, 
  std::vector<ArcQuad>& arcQuads
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
  std::vector<ArcSegment>& arcSegments, 
  std::vector<ArcTriangle>& arcTriangles, 
  std::vector<ArcQuad>& arcQuads
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
  std::vector<ArcSegment>&   arcSegments,
  std::vector<ArcTriangle>&  arcTriangles, 
  std::vector<ArcQuad>&      arcQuads) {

  addC(p + scale * glm::vec3(-1.0f, 0.0f, 0.0f), angle, scale, arcSegments, arcTriangles, arcQuads);
  addS(p + scale * glm::vec3(0.0f, 0.0f, 0.0f),  angle, scale, arcSegments, arcTriangles, arcQuads);
  addT(p + scale * glm::vec3(1.0f, 0.0f, 0.0f),  angle, scale, arcSegments, arcTriangles, arcQuads);

}

static void sampleDataGenerator(std::vector<ArcSegment>& arcSegs, std::vector<ArcTriangle> arcTriangles, std::vector<ArcQuad>& arcQuads) {

}

#endif // __SAMPLE_DATA_GENERATOR_H__