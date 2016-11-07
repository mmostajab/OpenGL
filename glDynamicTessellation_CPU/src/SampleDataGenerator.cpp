#include "SampleDataGenerator.h"

void addLogo(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>&   arcSegs,
  std::vector<ArcRep::ArcTriangle>&  arcTriangles,
  std::vector<ArcRep::ArcQuad>&      arcQuads
) {

  std::vector<Matrix4x4f> matrixSequence;

  matrixSequence.push_back(UnifiedMath::translate(p));
  matrixSequence.push_back(UnifiedMath::rotate(angle, Vector3Df(0.0f, 0.0f, 1.0f)));
  matrixSequence.push_back(UnifiedMath::scale(Vector3Df(scale, scale, scale)));
  matrixSequence.push_back(UnifiedMath::translate(Vector3Df(0.0f, 0.5f, 0.0f)));
  matrixSequence.push_back(UnifiedMath::scale(Vector3Df(1.0f / 2.0f, 1.0f / 2.0f, 1.0f)));

  Matrix4x4f shapeTransformation = UnifiedMath::matrixSequenceMultiply(matrixSequence);

  ArcRep::ArcTriangle arc[4];
  
  arc[0].set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 1.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.0f, 0.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.0f, 1.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0.0f)),
    100
  );
  arcTriangles.push_back(arc[0]);

  arc[1].set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.0f, 0.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -1.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.0f, -1.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0.0f)),
    100
  );
  arcTriangles.push_back(arc[1]);

  arc[3].set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 1.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.0f, 0.0f, 0.0f)),
    
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.0f, 1.0f, 0.0f)),
    100
  );
  arcTriangles.push_back(arc[3]);

  arc[2].set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.0f, 0.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df( 0.0f, -1.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df( 0.0f, 0.0f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.0f, -1.0f, 0.0f)),
    100
  );
  arcTriangles.push_back(arc[2]);

  
}

static void addC(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
) {

  // BBOX = MIN[-2.0f, -2.0f] MAX[1.4f, 2.0f]

  std::vector<Matrix4x4f> matrixSequence;

  matrixSequence.push_back(UnifiedMath::translate(p));
  matrixSequence.push_back(UnifiedMath::rotate(angle, Vector3Df(0.0f, 0.0f, 1.0f)));
  matrixSequence.push_back(UnifiedMath::scale(Vector3Df(scale, scale, scale)));
  matrixSequence.push_back(UnifiedMath::translate(Vector3Df(.0f, 0.5f, 0.0f)));
  matrixSequence.push_back(UnifiedMath::scale(Vector3Df(1.0f / 3.4f, 1.0f / 4.0f, 1.0f)));
  matrixSequence.push_back(UnifiedMath::translate(Vector3Df(+0.3f, 0.0f, 0.0f)));

  Matrix4x4f shapeTransformation = UnifiedMath::matrixSequenceMultiply(matrixSequence);

  ArcRep::ArcSegment arcSegment0, arcSegment1;

  arcSegment0.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.4f, 1.4f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.7f, 0.7f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.05f, 1.05f, 0.0f)),
    100
    );
  arcSegments.push_back(arcSegment0);

  arcSegment1.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.7f, -0.7f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.4f, -1.4f, 0.0f)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.05f, -1.05f, 0.0f)), 
    100);
  arcSegments.push_back(arcSegment1);

  ArcRep::ArcQuad arcQuad0, arcQuad1, arcQuad2, arcQuad3;
  arcQuad0.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 1.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.7f, 0.7f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0)),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 2.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.4f, 1.4f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0)),

    100);
  arcQuads.push_back(arcQuad0);

  arcQuad1.set(
    
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -1.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 1.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0)),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -2.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 2.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0)),

    100);
  arcQuads.push_back(arcQuad1);

  arcQuad2.set(
    
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.7f, -0.7f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -1.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0)),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.4f, -1.4f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -2.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 0.0f, 0)),

    100);
  arcQuads.push_back(arcQuad2);
}

static void addS(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
) {

  // BBOX = MIN[-2.0f, -5.0f] MAX[2.0f, +2.0f]

  std::vector<Matrix4x4f> matrixSequence;

  matrixSequence.push_back(UnifiedMath::translate(p));
  matrixSequence.push_back(UnifiedMath::rotate(angle, Vector3Df(0.0f, 0.0f, 1.0f)));
  matrixSequence.push_back(UnifiedMath::scale(Vector3Df(scale, scale, scale)));
  matrixSequence.push_back(UnifiedMath::translate(Vector3Df(.0f, 0.5f, 0.0f)));

  Matrix4x4f shapeTransformation = UnifiedMath::matrixSequenceMultiply(matrixSequence);
  
  float s_width = 0.2f;

  float outer_radius = (0.5f + s_width / 2.0f) / 2.0f;
  float iner_radius = ((0.5f - s_width) - s_width / 2.0f) / 2.0f;

  Vector3Df outer_center(0.0f, 0.5f - outer_radius, 0.0f);
  Vector3Df iner_center(0.0f, 0.5f - s_width - iner_radius, 0.0f);

  ArcRep::ArcSegment arcSegment0, arcSegment1;

  arcSegment0.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(outer_radius, outer_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(iner_radius, iner_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, (0.5f * Vector3Df(outer_radius + iner_radius, (iner_center[1] + outer_center[1]), 0.0f))),
    100);
  arcSegments.push_back(arcSegment0);

  arcSegment1.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-outer_radius, -outer_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-iner_radius, -iner_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, (-0.5f * Vector3Df(outer_radius + iner_radius, (iner_center[1] + outer_center[1]), 0.0f))),
    100);
  arcSegments.push_back(arcSegment1);

  ArcRep::ArcQuad arcQuad0, arcQuad1, arcQuad2, arcQuad3;
  arcQuad0.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-iner_radius, iner_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(iner_radius, iner_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, iner_center),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-outer_radius, outer_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(outer_radius, outer_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, outer_center),

    100);
  arcQuads.push_back(arcQuad0);

  arcQuad1.set(
    

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, iner_center[1] - iner_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-iner_radius, iner_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, iner_center),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, outer_center[1] - outer_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-outer_radius, outer_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, outer_center),

    100);
  arcQuads.push_back(arcQuad1);

  arcQuad2.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, outer_center[1] - outer_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -iner_center[1] - iner_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, -iner_center),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, iner_center[1] - iner_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -outer_center[1] - outer_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, -outer_center),

    100);
  arcQuads.push_back(arcQuad2);

  arcQuad3.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -iner_center[1] - iner_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-iner_radius, -iner_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, -iner_center),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -outer_center[1] - outer_radius, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-outer_radius, -outer_center[1], 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, -outer_center),

    100);

  arcQuads.push_back(arcQuad3);
}

static void addT(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
) {

  // BBOX = MIN[-2.0f, 0.0f] MAX[2.0f, 5.6f]
  
  std::vector<Matrix4x4f> matrixSequence;

  matrixSequence.push_back(UnifiedMath::translate(p));
  matrixSequence.push_back(UnifiedMath::rotate(angle, Vector3Df(0.0f, 0.0f, 1.0f)));
  matrixSequence.push_back(UnifiedMath::scale(Vector3Df(scale, scale, scale)));
  matrixSequence.push_back(UnifiedMath::translate(Vector3Df(.0f, 0.5f, 0.0f)));
  matrixSequence.push_back(UnifiedMath::scale(Vector3Df(1.0f / 4.0f, 1.0f / 5.6f, 1.0f)));
  matrixSequence.push_back(UnifiedMath::translate(Vector3Df(0.0f, -2.8, 0.0f)));


  Matrix4x4f shapeTransformation = UnifiedMath::matrixSequenceMultiply(matrixSequence);

  // (-2.0f, -3.0f) => (2.0f, 1.2f) 

  ArcRep::ArcTriangle arcTriangle0, arcTriangle1;
  arcTriangle0.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.0f, 4.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-2.0f, 4.1f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.8f, 4.2f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.4f, 4.2f, 0)),
    100);
  arcTriangles.push_back(arcTriangle0);

  arcTriangle1.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(2.0f, 4.1f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.0f, 4.0f, 0)),    
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.8f, 4.2f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(1.4f, 4.2f, 0)),
    100);
  arcTriangles.push_back(arcTriangle1);

  ArcRep::ArcQuad arcQuad0, arcQuad1;
  arcQuad0.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.0f, 4.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(+1.0f, 4.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 3.0f, 0)),
    
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-1.8f, 4.2f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(+1.8f, 4.2f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 3.0f, 0)),

    100);
  arcQuads.push_back(arcQuad0);

  arcQuad1.set(
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-0.2f, 0.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.2f, 0.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, -1.0f, 0)),

    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(-0.4f, 4.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.4f, 4.0f, 0)),
    UnifiedMath::TransformWithTranslation(shapeTransformation, Vector3Df(0.0f, 3.0f, 0)),

    100);
  arcQuads.push_back(arcQuad1);
}

void addM(
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
) {
  ArcRep::ArcQuad arcQuad0, arcQuad1, arcQuad2;
  arcQuad0.set(
    Vector3Df(+1.4f, 1.4f, 0),
    Vector3Df(-1.4f, 1.4f, 0),
    Vector3Df(0.0f, 0.0f, 0),

    Vector3Df(+1.4f, 1.0f, 0),
    Vector3Df(-1.4f, 1.0f, 0),
    Vector3Df(0.0f, 0.0f, 0),
    100);
  arcQuads.push_back(arcQuad0);

  arcQuad1.set(
    Vector3Df(+1.8f, +1.4f, 0),
    Vector3Df(+1.4f, +1.4f, 0),
    Vector3Df(+1.6f, 1.6f, 0),

    Vector3Df(+1.8f, -2.0f, 0),
    Vector3Df(+1.4f, -2.0f, 0),
    Vector3Df(+1.6f, -1.8f, 0),
    100
  );

  arcQuads.push_back(arcQuad1);

  arcQuad2.set(
    Vector3Df(-1.4f, +1.4f, 0),
    Vector3Df(-1.8f, +1.4f, 0),
    Vector3Df(-1.6f, 1.6f, 0),

    Vector3Df(-1.4f, -2.0f, 0),
    Vector3Df(-1.8f, -2.0f, 0),
    Vector3Df(-1.6f, -1.8f, 0),
    100

  );
  arcQuads.push_back(arcQuad2);
}

void addF(
  std::vector<ArcRep::ArcSegment>& arcSegments, 
  std::vector<ArcRep::ArcTriangle>& arcTriangles, 
  std::vector<ArcRep::ArcQuad>& arcQuads
) {
  ArcRep::ArcQuad arcQuad0, arcQuad1, arcQuad2;
  arcQuad0.set( 
    Vector3Df(-2.0f, 2.0f, 0),
    Vector3Df(+2.0f, 2.0f, 0),
    Vector3Df(0.0f, 2.3f, 0),

    Vector3Df(-2.0f, 1.4f, 0),
    Vector3Df(+2.0f, 1.4f, 0),
    Vector3Df(0.0f, 29.5f, 0),
    100
  );
  arcQuads.push_back(arcQuad0);

  arcQuad1.set(
    Vector3Df(+1.0f, -0.5f, 0),
    Vector3Df(-2.0f, -0.5f, 0),
    Vector3Df(-0.5f, -0.7f, 0),

    Vector3Df(+1.0f, -1.0f, 0),
    Vector3Df(-2.0f, -1.0f, 0),
    Vector3Df(-0.5f, -1.3f, 0),

    100);
  arcQuads.push_back(arcQuad1);
  
  arcQuad2.set(
    Vector3Df(-2.4f, +2.0f, 0),
    Vector3Df(-2.0f, +2.0f, 0),
    Vector3Df(-2.2f, 3.0f, 0),

    Vector3Df(-2.4f, -4.0f, 0),
    Vector3Df(-2.0f, -4.0f, 0),
    Vector3Df(-2.2f, -5.0f, 0),

    100);
  arcQuads.push_back(arcQuad2);
}

void addCST(
  Vector3Df p, float angle, float scale,
  std::vector<ArcRep::ArcSegment>&   arcSegments,
  std::vector<ArcRep::ArcTriangle>&  arcTriangles, 
  std::vector<ArcRep::ArcQuad>&      arcQuads) {

  addC(p + scale * Vector3Df(-1.0f, 0.0f, 0.0f), angle, scale, arcSegments, arcTriangles, arcQuads);
  addS(p + scale * Vector3Df(0.0f, 0.0f, 0.0f),  angle, scale, arcSegments, arcTriangles, arcQuads);
  addT(p + scale * Vector3Df(1.0f, 0.0f, 0.0f),  angle, scale, arcSegments, arcTriangles, arcQuads);

}

void sampleDataGenerator(std::vector<ArcRep::ArcSegment>& arcSegs, std::vector<ArcRep::ArcTriangle> arcTriangles, std::vector<ArcRep::ArcQuad>& arcQuads) {

}