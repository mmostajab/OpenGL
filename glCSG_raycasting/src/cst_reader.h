#ifndef __CST_READER_H__
#define __CST_READER_H__

// STD
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

// GLM
#include <glm/glm.hpp>

#include "AABB.h"
#include "helper.h"

// Colors
const glm::vec3 CST_COL_YELLOW  ( 128.f / 255.f,    100.f / 255.f,      0.0f           );
const glm::vec3 CST_COL_MAGENTA ( 192.f / 255.f,    0.0f,               192.f / 255.f  );
const glm::vec3 CST_COL_GREEN   ( 0.0f,             192.f / 255.f,      0.0f           );
const glm::vec3 CST_COL_BOARD   ( 26.0f / 255.f,    121.f / 255.f,      101.0f / 255.f );

//Vec3fa operator+(const glm::vec3& v1, const float& v2) {
//	return glm::vec3(v1.x + v2, v1.y + v2, v1.z + v2);
//}
//
//Vec3fa operator-(const glm::vec3& v1, const float& v2) {
//	return glm::vec3(v1.x - v2, v1.y - v2, v1.z - v2);
//}

struct CSTPrimitive
{
	std::string					type;
	int							id;
	std::vector<glm::vec2>	vertices;
	float						width;
	float						center;
	float						posOffset;
	float						negOffset;
	int							sign;
	glm::vec3              color;

	AABB                        bbox;
	glm::vec3              meta;

	int							shape;
};

void createPrimitive(
	const AABB& bbox, glm::vec3 color, std::string shape_type, int sign, int shapeID,
	std::vector<CSTPrimitive>& _primitives);

void createPrimitive(
	const AABB& bbox, glm::vec3 color, std::string shape_type, int sign, int shapeId, glm::vec3 meta,
	std::vector<CSTPrimitive>& _primitives);

void createDirectedBox(glm::vec3 start, glm::vec3 end, glm::vec3 radius, glm::vec3 color, int shape, int sign, std::vector<CSTPrimitive>& _primitives);

void createCylinder(
	glm::vec3 center, glm::vec3 radius, glm::vec3 color, int shape, int sign,
	std::vector<CSTPrimitive>& _primitives);

void createRoundedLine(
	glm::vec3 start, glm::vec3 end, glm::vec3 radius,
	glm::vec3 color, int shape, int sign, int caps,
	std::vector<CSTPrimitive>& _primitives);

void readModelCST(std::vector<CSTPrimitive>& _cstPrimitives, std::string filename)
{
	const bool SKIP_POSITIVE = 0;
	const bool SKIP_NEGATIVE = 0;
	const bool SKIP_POLYGONS = 0;
	const bool SKIP_NONPOLYGONS = 0;

	if (filename.empty())
	{
		filename = "plasma.cst";
		filename = "C:/Users/adietric/Desktop/gulu";
		filename = "C:/Users/adietric/Desktop/plasma.cst";
	}

	AABB sceneBBox;
	sceneBBox.uninit();

	int shape = 0;

	std::vector<CSTPrimitive> primitives;
	std::map<std::string, size_t> types;

	std::ifstream in(filename);
	std::string token;

	in >> token; // '['
	while (in >> token)
	{
		if (token == "]")
			break;

		CSTPrimitive primitive;

		in >> primitive.type;
		in >> primitive.id;
		in >> primitive.width;
		in >> primitive.center;
		in >> primitive.posOffset;
		in >> primitive.negOffset;
		in >> primitive.sign;
		in >> token; // '['

		if (primitive.posOffset == 0 && primitive.negOffset == 0)
		{
			primitive.posOffset = +5 * 00.1;
			primitive.negOffset = -5 * 00.1;

			if (primitive.type == "CIRCLE" && primitive.sign < 0)
			{
				primitive.posOffset += 100;
				primitive.negOffset -= 100;
			}
		}

		if (primitive.type != "CIRCLE")
		{
			primitive.posOffset *= 10;
			primitive.negOffset *= 10;
		}

		while (in >> token)
		{
			if (token[0] == ']')
				break;

			glm::vec2 point;
			sscanf(token.c_str(), "(%f,%f)", &(point.x), &(point.y));
			primitive.vertices.push_back(point);
		}

		primitive.shape = shape++;
		primitives.push_back(primitive);

		if ((SKIP_POSITIVE && (primitive.sign > 0))
			|| (SKIP_NEGATIVE && (primitive.sign < 0))
			|| (SKIP_POLYGONS && (primitive.type == "POLYGON"))
			|| (SKIP_NONPOLYGONS && (primitive.type != "POLYGON")))
		{
			primitives.pop_back();
		}

		in >> token; // ']'
	}

	for (size_t i = 0; i < primitives.size(); i++)
	{
		const CSTPrimitive &primitive = primitives[i];
		
		types[primitive.type]++;

		const float top = primitive.center + primitive.posOffset;
		const float bot = primitive.center + primitive.negOffset;

		int sign = primitive.sign;

		if (primitive.type == "LINE") {
			for (size_t j = 0; j < primitive.vertices.size() - 1; j++)
			{
				glm::vec3 start = glm::vec3(primitive.vertices[j].x, (top + bot) / 2, primitive.vertices[j].y);
				glm::vec3 end = glm::vec3(primitive.vertices[j + 1].x, (top + bot) / 2, primitive.vertices[j + 1].y);
				glm::vec3 radius = 0.5f* glm::vec3(primitive.width, (top - bot) / 2, primitive.width);

				int caps = j < primitive.vertices.size() - 2 ? 1 : 2;
				int shape = primitive.shape;

				//createRoundedLine(start, end, radius, CST_COL_YELLOW, shape, sign, caps);
				createRoundedLine(start, end, radius, CST_COL_YELLOW, shape, sign, caps, _cstPrimitives);
				sceneBBox.addpoint(start);
				sceneBBox.addpoint(end);
			}
		}
		else if (primitive.type == "CIRCLE") {
			glm::vec3 center = glm::vec3(primitive.vertices[0].x, (top + bot) / 2, primitive.vertices[0].y);
			glm::vec3 radius = 0.5f* glm::vec3(primitive.width, (top - bot) / 2, primitive.width);

			if (primitive.sign > 0)
				//createCylinder(center, radius, CST_COL_MAGENTA, shape, sign);
				createCylinder(center, radius, CST_COL_MAGENTA, shape, sign, _cstPrimitives);
			else
				//createCylinder(center, radius, CST_COL_GREEN, shape, sign);
				createCylinder(center, radius, CST_COL_GREEN, shape, sign, _cstPrimitives);
			
			sceneBBox.addpoint(center - radius);
			sceneBBox.addpoint(center + radius);
		}
		else if (primitive.type == "POLYGON") {

			_cstPrimitives.push_back(primitive);
			_cstPrimitives.back().color = CST_COL_BOARD;

		}
	}

	std::cout << "\nImported from \"" << filename << "\":" << std::endl;
	for (auto &it : types)
		printf("%10s : %10d\n", it.first.c_str(), static_cast<int>(it.second));

	std::cout << "\nCreated: " << std::endl
		<< "  " << _cstPrimitives.size() << " CSG primitives" << std::endl;

	std::cout << "\nScene Box: " << std::endl << " ( " 
    << sceneBBox.min.x << " " << sceneBBox.min.y << " " << sceneBBox.min.z << " , " 
    << sceneBBox.max.x << " " << sceneBBox.max.y << " " << sceneBBox.max.z << " ) " << std::endl;
}

void createPrimitive(
	const AABB& bbox, glm::vec3 color, std::string shape_type, int shapeID, int sign,
	std::vector<CSTPrimitive>& _primitives)
{
	CSTPrimitive primitive;

	primitive.color = color;
	primitive.type = shape_type;
	primitive.shape = shapeID;
	primitive.sign = sign;
	
	primitive.bbox = bbox;

	_primitives.push_back(primitive);
}

void createPrimitive(
	const AABB& bbox, glm::vec3 color, std::string shape_type, int sign, int shapeId, glm::vec3 meta,
	std::vector<CSTPrimitive>& _primitives)
{
	CSTPrimitive primitive;

	primitive.color = color;
	primitive.type = shape_type;
	primitive.sign = sign;
	primitive.shape = shapeId;

	primitive.bbox = bbox;
	primitive.meta = meta;

	_primitives.push_back(primitive);
}

void createBox(glm::vec3 center, glm::vec3 radius, glm::vec3 color, int shape, int sign, std::vector<CSTPrimitive>& _primitives)
{
	const glm::vec3 min = center - radius;
	const glm::vec3 max = center + radius;

	AABB bbox;
	bbox.min = min;
	bbox.max = max;

	createPrimitive(bbox, color, "BOX", shape, sign, _primitives);
}

void createDirectedBox(glm::vec3 start, glm::vec3 end, glm::vec3 radius, glm::vec3 color, int shape, int sign, std::vector<CSTPrimitive>& _primitives) {
	AABB bbox;
	bbox.min[0] = start.x;	bbox.max[0] = end.x;
	bbox.min[1] = start.y;	bbox.max[1] = end.y;
	bbox.min[2] = start.z;	bbox.max[2] = end.z;

	createPrimitive(bbox, color, "DIRECTED_BOX", sign, shape, radius, _primitives);
}

void createLine(glm::vec3 start, glm::vec3 end, glm::vec3 radius, glm::vec3 color, int shape, int sign, std::vector<CSTPrimitive>& _primitives)
{
	const glm::vec3 center = 0.5f * (end + start);
	const glm::vec3 extension = 0.5f * (end - start);

	radius += glm::vec3(fabsf(extension.x), fabsf(extension.y), fabsf(extension.z));
	createBox(center, radius, color, shape, sign, _primitives);
}

void createCylinder(
	glm::vec3 center, glm::vec3 radius, glm::vec3 color, int shape, int sign,
	std::vector<CSTPrimitive>& _primitives)
{
	AABB bbox;
	bbox.min = center - radius;
	bbox.max = center + radius;

	createPrimitive(bbox, color, "CYLINDER", shape, sign, _primitives);
}

void createRoundedLine(
	glm::vec3 start, glm::vec3 end, glm::vec3 radius,
	glm::vec3 color, int shape, int sign, int caps,
	std::vector<CSTPrimitive>& _primitives) {

	CSTPrimitive primitive;

	if (caps > 0) createCylinder(start, radius, color, shape, sign, _primitives);
	if (caps > 1) createCylinder(end, radius, color, shape, sign, _primitives);

	glm::vec3 dir = glm::normalize(end - start);
	const glm::ivec3 abs_dir = glm::ivec3(fabsf(dir.x), fabsf(dir.y), fabsf(dir.z));
  if (!((abs_dir == glm::ivec3(1, 0, 0)) || (abs_dir == glm::ivec3(0, 1, 0)) || (abs_dir == glm::ivec3(0, 0, 1)))) {
		// Create directed box
		createDirectedBox(start, end, radius, color, shape, sign, _primitives);
	}
	else
	{
		// Create Line
		// createLine(start+(direction*radius), end-(direction*radius), radius, color, shape, medium);
		//std::cout << "WARNING: create line case in not implemented!\n";
		createLine(start + (dir*radius), end - (dir*radius), radius, color, shape, sign, _primitives);
	}
}

#endif // __CST_READER_H__