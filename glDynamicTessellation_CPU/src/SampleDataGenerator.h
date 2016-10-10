#ifndef __SAMPLE_DATA_GENERATOR_H__
#define __SAMPLE_DATA_GENERATOR_H__

#include "ArcSegment.h"
#include "ArcTriangle.h"
#include "ArcQuad.h"

void addLogo(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>&   arcSegs,
  std::vector<ArcRep::ArcTriangle>&  arcTriangles,
  std::vector<ArcRep::ArcQuad>&      arcQuads
);

void addC(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
);

void addS(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
);

void addT(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
);

void addM(
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
);

void addF(
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
);

void addCST(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>&   arcSegments,
  std::vector<ArcRep::ArcTriangle>&  arcTriangles, 
  std::vector<ArcRep::ArcQuad>&      arcQuads);

static void sampleDataGenerator(std::vector<ArcRep::ArcSegment>& arcSegs, std::vector<ArcRep::ArcTriangle> arcTriangles, std::vector<ArcRep::ArcQuad>& arcQuads);

#endif // __SAMPLE_DATA_GENERATOR_H__