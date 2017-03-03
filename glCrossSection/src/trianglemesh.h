#pragma once

// STD
#include <vector>
#include <stdint.h>

// GLM
#include <glm/glm.hpp>

#include "idrawablebase.h"

namespace VisUtils {

	struct TriangleMeshVertex {
		glm::uint32     id;
		glm::vec3		position;
		glm::vec3       normal;
		glm::vec4       result;
		 
		TriangleMeshVertex(
				const uint32_t& _id = 0, 
				const double& _x  = 0.0,	const double& _y  = 0.0,	const double& _z  = 0.0, 
				const double& _Nx = 0.0,	const double& _Ny = 0.0,	const double& _Nz = 0.0, 
				const double& _Rx = 0.0,	const double& _Ry = 0.0,	const double& _Rz = 0.0, const double& _Rw = 0.0) {
			id			= _id;
			position	= glm::vec3(_x, _y, _z);
			normal		= glm::vec3(_Nx, _Ny, _Nz);
			result		= glm::vec4(_Rx, _Ry, _Rz, _Rw);
		}
	};

	class TriangleMesh : public IDrawableBase {
	public:
		TriangleMesh(const std::vector<TriangleMeshVertex>& _verticesPool, const std::vector<uint32_t>& _indicesPool);

		virtual void compileShaders()     override;
		virtual void createBuffer()       override;
		virtual void draw()               override;
		virtual void updateBuffer()       override;
		virtual void freeBuffer()         override;

		~TriangleMesh();

		void calculateNormals();

	private:

		// Vertices Pool
		std::vector<TriangleMeshVertex>     m_triangleMeshVertices;
		std::vector<uint32_t>				m_triangleMeshIndices;

		// Particle GL Buffer
		GLuint								m_glTriangleMeshVAO;
		GLuint								m_glTriangleMeshVerticesBuffer;
		GLuint								m_glTriangleMeshIndicesBuffer;

		// Particle Rendering shader
		GLuint								m_glTriangleMeshRenderProgram;

	};

}
