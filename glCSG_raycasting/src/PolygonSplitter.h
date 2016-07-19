/**
 *
 * Polygon Splitter
 *
 * Subdivides planar polygons into a set of smaller polygon with fewer vertices.
 *
 */
 
#pragma once

// STD
#include <algorithm>
#include <limits>
#include <vector>

// CLib
#include <cstdio>

// Boost
#include <boost/geometry/geometry.hpp>
//#include "e:/Morteza/Dependencies/boost_1_61_0/boost/geometry/geometry.hpp"

// RPE
#include "AABB.h"


class PolygonSplitter
{
public:

	/// Generic point types.
	typedef struct {float x, y;} Point;

	/// Geometry sets.
	typedef std::vector<Point>   Points;
	typedef std::vector<Point>   Polygon;
	typedef std::vector<Polygon> Polygons;
	typedef std::vector<AABB2D>  Boxes;

	/// Initialize with splitting parameters.
	inline PolygonSplitter( unsigned maxPointsPerCell  = 10,
		                    unsigned maxRecursionDepth = 100,
	                        bool     printLogToStdout  = false );

	/// Split an input polygon into a set of smaller polygons.
	inline Polygons SplitPolygon( const Polygon &polygon );

private:

	/// Subdivide a set of points within a given bounding box (quad-tree).
	inline Boxes ComputeClipBoxes( const Points &points, const AABB2D &box, unsigned level=0 );

	/// Clip an input polygon against an axis-aligned bounding box.
	inline Polygons ClipPolygon( const Polygon &polygon, const AABB2D &box );

	/// Splitting parameters.
	unsigned m_maxPointsPerCell;
	unsigned m_maxRecursionDepth;
	bool     m_printLogToStdout;
};


// ****************************************************************************

inline PolygonSplitter::PolygonSplitter( unsigned maxPointsPerCell, unsigned maxRecursionDepth, bool printLogToStdout )
{
	m_maxPointsPerCell  = maxPointsPerCell;
	m_maxRecursionDepth = maxRecursionDepth;
	m_printLogToStdout  = printLogToStdout;
}

// ----------------------------------------------------------------------------

inline PolygonSplitter::Polygons PolygonSplitter::ClipPolygon( const Polygon &polygonIn, const AABB2D &box )
{
	Polygons polygonsOut;

	//
	// Use boost::geometry for clipping
	// 

	enum {COUNTER_CLOCKWISE = 0, CLOCKWISE = 1};
	enum {OPEN              = 0, CLOSED    = 1};

	typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> BoostPoint;
	typedef boost::geometry::model::polygon<BoostPoint, COUNTER_CLOCKWISE, OPEN>	BoostPolygon;
	typedef boost::geometry::model::box<BoostPoint>                                 BoostBox;

	BoostPolygon boostPolygonIn;
	for (auto &point : polygonIn)
		boost::geometry::append(boostPolygonIn, BoostPoint(point.x, point.y));

	BoostBox boostBox(BoostPoint(box.min[0], box.min[1]), BoostPoint(box.max[0], box.max[1]));

	std::vector<BoostPolygon> boostPolygonsOut;
	boost::geometry::intersection(boostBox, boostPolygonIn, boostPolygonsOut);

	for (auto &boostPolygon : boostPolygonsOut)
	{
		Polygon polygonOut;
		for (auto &boostPoint : boostPolygon.outer())
		{
			Point point = { boost::geometry::get<0>(boostPoint), boost::geometry::get<1>(boostPoint) };
			polygonOut.push_back(point);
		}

		polygonsOut.push_back(polygonOut);
	}

	return polygonsOut;
}

// ----------------------------------------------------------------------------

inline PolygonSplitter::Polygons PolygonSplitter::SplitPolygon( const Polygon &polygon )
{
	Polygons result;

	AABB2D boundingBox;
	for (auto &point : polygon)
		boundingBox.extend(point.x, point.y);

	Boxes clipBoxes = ComputeClipBoxes(polygon, boundingBox);

	for (auto &box : clipBoxes)
	{
		Polygons polygons = ClipPolygon(polygon, box);
		result.insert(result.end(), polygons.begin(), polygons.end());
	}

	return result;
}

// ----------------------------------------------------------------------------

inline PolygonSplitter::Boxes PolygonSplitter::ComputeClipBoxes( const Points &points, const AABB2D &box, unsigned level )
{
	if (m_printLogToStdout)
	{
		for (unsigned i=0; i<level; i++)
			printf("  ");
//		printf(" [%d] Clip Box: (%f,%f) (%f,%f), Points: %u\n", level, box.min[0],box.min[1],box.max[0],box.max[1], points.size());
	}

	Boxes result;

	if (points.size() <= m_maxPointsPerCell)
	{
		result.push_back(box);
	}
	else
	{
		AABB2D boxLeft  = box;
		AABB2D boxRight = box;

		size_t d = (size_t)(box.max[1]-box.min[1] > box.max[0]-box.min[0]);
		boxLeft.max[d] = boxRight.min[d] = 0.5f * (box.min[d] + box.max[d]);

		Points pointsLeft;
		Points pointsRight;

		for (auto &point : points)
		{
			if (boxLeft.contains((float*)(&point)))
				pointsLeft.push_back(point);
			if (boxRight.contains((float*)(&point)))
				pointsRight.push_back(point);
		}
	
		Boxes boxesLeft  = ComputeClipBoxes(pointsLeft, boxLeft, level+1);
		Boxes boxesRight = ComputeClipBoxes(pointsRight, boxRight, level+1);

		result.insert(result.end(), boxesLeft.begin(), boxesLeft.end());
		result.insert(result.end(), boxesRight.begin(), boxesRight.end());
	}

	return result;
}
