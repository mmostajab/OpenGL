#include "trianglemesh.h"

// Std
#include <iostream>

#include "helper.h"

VisUtils::TriangleMesh::TriangleMesh(const std::vector<TriangleMeshVertex>& _verticesPool, const std::vector<uint32_t>& _indicesPool)
{
	m_triangleMeshVertices	= _verticesPool;
	m_triangleMeshIndices	= _indicesPool;
}

void VisUtils::TriangleMesh::compileShaders()
{
	m_glTriangleMeshRenderProgram = compile_link_vs_fs("../../src/glsl/trianglemesh/trianglemesh.vert", "../../src/glsl/trianglemesh/trianglemesh.frag");
}

void VisUtils::TriangleMesh::createBuffer()
{
	glCreateBuffers(1, &m_glTriangleMeshVerticesBuffer);
	if (m_glTriangleMeshVerticesBuffer <= 0) {
		assert(false && "Cannot create the buffer.\n");
		std::cout << "Cannot create the buffer." << std::endl;
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_glTriangleMeshVerticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_triangleMeshVertices.size() * sizeof(TriangleMeshVertex), m_triangleMeshVertices.data(), GL_STATIC_DRAW);

	glCreateBuffers(1, &m_glTriangleMeshIndicesBuffer);
	if (m_glTriangleMeshIndicesBuffer <= 0) {
		assert(false && "Cannot create the index buffer.\n");
		std::cout << "Cannot create the index buffer." << std::endl;
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glTriangleMeshIndicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangleMeshIndices.size() * sizeof(uint32_t), m_triangleMeshIndices.data(), GL_STATIC_DRAW);
}

void VisUtils::TriangleMesh::draw()
{
	glUseProgram(m_glTriangleMeshRenderProgram);
	int stride = sizeof(TriangleMeshVertex);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_glTriangleMeshVerticesBuffer);
	glVertexAttribIPointer((GLint)0, 1, GL_UNSIGNED_INT,			  stride, (char*)0 + offsetof(TriangleMeshVertex, id));
	glVertexAttribPointer((GLint)1, 3,	GL_FLOAT,			GL_FALSE, stride, (char*)0 + offsetof(TriangleMeshVertex, position));
	glVertexAttribPointer((GLint)2, 3,	GL_FLOAT,			GL_FALSE, stride, (char*)0 + offsetof(TriangleMeshVertex, normal));
	glVertexAttribPointer((GLint)3, 4,	GL_FLOAT,			GL_FALSE, stride, (char*)0 + offsetof(TriangleMeshVertex, result));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glTriangleMeshIndicesBuffer);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_triangleMeshIndices.size()), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

}

void VisUtils::TriangleMesh::updateBuffer()
{
}

void VisUtils::TriangleMesh::freeBuffer()
{
	if (m_glTriangleMeshVerticesBuffer > 0) glDeleteBuffers(1, &m_glTriangleMeshVerticesBuffer);	m_glTriangleMeshVerticesBuffer	= 0;
	if (m_glTriangleMeshIndicesBuffer  > 0) glDeleteBuffers(1, &m_glTriangleMeshIndicesBuffer);	m_glTriangleMeshIndicesBuffer	= 0;

	glDeleteProgram(m_glTriangleMeshRenderProgram);
}

VisUtils::TriangleMesh::~TriangleMesh()
{
	freeBuffer();
}

void VisUtils::TriangleMesh::calculateNormals()
{
	for (int i = 0; i < m_triangleMeshIndices.size(); i += 3)
	{
		TriangleMeshVertex v0 = m_triangleMeshVertices[m_triangleMeshIndices[i]];
		TriangleMeshVertex v1 = m_triangleMeshVertices[m_triangleMeshIndices[i + 1]];
		TriangleMeshVertex v2 = m_triangleMeshVertices[m_triangleMeshIndices[i + 2]];

		glm::vec3 vec1 = v1.position - v0.position;
		glm::vec3 vec2 = v2.position - v0.position;
		glm::vec3 normal = glm::cross(vec1, vec2);

		m_triangleMeshVertices[m_triangleMeshIndices[i]].normal += normal;
		m_triangleMeshVertices[m_triangleMeshIndices[i + 1]].normal += normal;
		m_triangleMeshVertices[m_triangleMeshIndices[i + 2]].normal += normal;
	}

	for (auto &v : m_triangleMeshVertices)
	{
		v.normal = glm::normalize(v.normal);
	}
	
}
