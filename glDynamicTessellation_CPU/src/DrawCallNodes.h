#ifndef __DRAW_CALL_NODES_H__
#define __DRAW_CALL_NODES_H__

#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGNodePtr.h>
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGGeoPositionsFields.h>
#include <OpenSG/OSGGeoNormalsFields.h>
#include <OpenSG/OSGGeoColorsFields.h>
#include <OpenSG/OSGPerspectiveCamera.h>
#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGChunkMaterial.h>
#include <OpenSG/OSGPolygonChunk.h>
#include <OpenSG/OSGLineChunk.h>
#include <OpenSG/OSGPointChunk.h>


static OSG::NodePtr createDrawArraysNode(
	OSG::Vec3f translation,

	const std::vector<OSG::Pnt3f>&  positions,
	const std::vector<OSG::UInt32>& lengths,
	const std::vector<OSG::UInt8>&  types) {

	OSG::NodePtr node = OSG::Node::create();
	OSG::GeometryPtr geo = OSG::Geometry::create();

	OSG::SimpleMaterialPtr mat = OSG::SimpleMaterial::create();
	mat->setAmbient(OSG::Color3f(0.15f, 0.15f, 0.15f));
	mat->setDiffuse(OSG::Color3f(0.3f, 0.8f, 0.2f));
	mat->setSpecular(OSG::Color3f(0.6f, 0.8f, 0.4f));

	OSG::GeoPositions3fPtr vertex_positions = OSG::GeoPositions3f::create();
	OSG::GeoNormals3fPtr   vertex_normals   = OSG::GeoNormals3f::create();
	OSG::GeoColors3fPtr    vertex_colors    = OSG::GeoColors3f::create();
	for(size_t i = 0; i < positions.size(); i++) {
		vertex_positions->addValue(OSG::Pnt3f(positions[i]));
		vertex_normals->addValue( OSG::Vec3f( 0.0f, 0.0f, 1.0f ) );
		vertex_colors->addValue(OSG::Color3f( 1.0f, 1.0f, 0.0f) );//static_cast<float>(rand() % 1000) / 1000.0f, static_cast<float>(rand() % 1000) / 1000.0f, static_cast<float>(rand() % 1000) / 1000.0f ) ) ;
	}

	//OSG::GeoIndicesUI32Ptr primitive_indices = OSG::GeoIndicesUI32::create();
	//for(size_t i = 0; i < positions.size(); i++) primitive_indices->addValue(static_cast<OSG::UInt32>(i));

	OSG::GeoPLengthsUI32Ptr primitve_lengths = OSG::GeoPLengthsUI32::create();
	for(size_t i = 0; i < lengths.size(); i++) primitve_lengths->addValue(lengths[i]);

	OSG::GeoPTypesUI8Ptr primitve_types = OSG::GeoPTypesUI8::create();
	for(size_t i = 0; i < types.size(); i++) primitve_types->addValue(types[i]);

	OSG::beginEditCP(geo);
	geo->setTypes(primitve_types);
	geo->setLengths(primitve_lengths);
	//geo->setIndices(primitive_indices);
	geo->setPositions(vertex_positions);
	geo->setColors(vertex_colors);
	geo->setNormals(vertex_normals);

	geo->setMaterial(mat);

	OSG::endEditCP(geo);

	

	node->setCore(geo);

	// create a transform node
	OSG::NodePtr transform = OSG::Node::create();
	OSG::TransformPtr transform_core = OSG::Transform::create();
	OSG::Matrix4f matrix;
	//matrix.identity();
	//matrix.setTranslate(translation);
	//transform_core->setMatrix(matrix);
	transform->setCore(transform_core);
	transform->addChild(node);

	return transform;
}

static void createDrawArraysNode(
	OSG::NodePtr transform,

	const std::vector<OSG::Pnt3f>&  positions,
	const std::vector<OSG::UInt32>& lengths,
	const std::vector<OSG::UInt8>&  types) {

	OSG::NodePtr node = OSG::Node::create();
	OSG::GeometryPtr geo = OSG::Geometry::create();

  OSG::PolygonChunkPtr polygonChunkWireframe = OSG::PolygonChunk::create();
	OSG::beginEditCP(polygonChunkWireframe);
	polygonChunkWireframe->setFrontMode(GL_LINE);
  polygonChunkWireframe->setBackMode(GL_LINE);
  polygonChunkWireframe->setFrontFace(GL_LINE);
  polygonChunkWireframe->setCullFace(GL_NONE);
	OSG::endEditCP  (polygonChunkWireframe);


	OSG::SimpleMaterialPtr mat = OSG::SimpleMaterial::create();
	OSG::beginEditCP(mat);
	mat->setAmbient(OSG::Color3f(0.15f, 0.15f, 0.15f));
	mat->setDiffuse(OSG::Color3f(0.3f, 0.8f, 0.2f));
	mat->setSpecular(OSG::Color3f(0.6f, 0.8f, 0.4f));
  mat->setLit(false);
  mat->addChunk(polygonChunkWireframe);
	OSG::endEditCP(mat);
   

	OSG::GeoPositions3fPtr vertex_positions = OSG::GeoPositions3f::create();
	OSG::GeoNormals3fPtr   vertex_normals   = OSG::GeoNormals3f::create();
	OSG::GeoColors3fPtr    vertex_colors    = OSG::GeoColors3f::create();
	for(size_t i = 0; i < positions.size(); i++) {
		float t = static_cast<float>(i) / static_cast<float>(positions.size());

		vertex_positions->addValue(OSG::Pnt3f(positions[i]));
		vertex_normals->addValue( OSG::Vec3f( 0.0f, 1.0f, 0.0f ) );
		vertex_colors->addValue( OSG::Color3f(t, t, t) ) ;
		//vertex_colors->addValue(OSG::Color3f( static_cast<float>(rand() % 1000) / 1000.0f, static_cast<float>(rand() % 1000) / 1000.0f, static_cast<float>(rand() % 1000) / 1000.0f ) ) ;
	}

	//OSG::GeoIndicesUI32Ptr primitive_indices = OSG::GeoIndicesUI32::create();
	//for(size_t i = 0; i < positions.size(); i++) primitive_indices->addValue(static_cast<OSG::UInt32>(i));

	OSG::GeoPLengthsUI32Ptr primitve_lengths = OSG::GeoPLengthsUI32::create();
	for(size_t i = 0; i < lengths.size(); i++) primitve_lengths->addValue(lengths[i]);

	OSG::GeoPTypesUI8Ptr primitve_types = OSG::GeoPTypesUI8::create();
	for(size_t i = 0; i < types.size(); i++) primitve_types->addValue(types[i]);

	OSG::beginEditCP(geo);

	geo->setTypes(primitve_types);
	geo->setLengths(primitve_lengths);
	//geo->setIndices(primitive_indices);
	geo->setPositions(vertex_positions);
	geo->setColors(vertex_colors);
	geo->setNormals(vertex_normals);
	geo->setMaterial(mat);

	OSG::endEditCP(geo);

	OSG::beginEditCP(node);
	node->setCore(geo);
	OSG::endEditCP(node);

	OSG::beginEditCP(transform);

  // Clear the old children
  osg::UInt32 nChilds = transform->getNChildren();
  for(osg::UInt32 i = 0; i < nChilds; i++) transform->subChild(i);
	
  // Add the new child
  transform->addChild(node);
	
  OSG::endEditCP(transform);
}

#endif // __DRAW_CALL_NODES_H__