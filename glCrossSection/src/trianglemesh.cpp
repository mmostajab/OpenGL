#include "trianglemesh.h"

// Std
#include <iostream>



VisUtils::TriangleMesh::TriangleMesh(const std::vector<TriangleMeshVertex>& _verticesPool, const std::vector<uint32_t>& _indicesPool)
{
	m_triangleMeshVertices	= _verticesPool;
	m_triangleMeshIndices	= _indicesPool;
}

void VisUtils::TriangleMesh::compileShaders()
{
	QGLShader vertexShader(QGLShader::Vertex);
	vertexShader.compileSourceFile("../media/shaders/trianglemesh/trianglemesh.vert");

	QGLShader fragmentShader(QGLShader::Fragment);
	fragmentShader.compileSourceFile("../media/shaders/trianglemesh/trianglemesh.frag");

	if (vertexShader.isCompiled() && fragmentShader.isCompiled()) {
		m_glTriangleMeshRenderProgram.addShader(&vertexShader);
		m_glTriangleMeshRenderProgram.addShader(&fragmentShader);
		m_glTriangleMeshRenderProgram.link();
	}
	else {
		assert(false && "Cannot compile the shaders.\n");
		std::cout << "ERROR: Cannot compile the shaders.\n";
		return;
	}
}

void VisUtils::TriangleMesh::createBuffer()
{
	QOpenGLFunctions_4_5_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glCreateBuffers(1, &m_glTriangleMeshVerticesBuffer);
	if (m_glTriangleMeshVerticesBuffer <= 0) {
		assert(false && "Cannot create the buffer.\n");
		std::cout << "Cannot create the buffer." << std::endl;
		return;
	}

	f->glBindBuffer(GL_ARRAY_BUFFER, m_glTriangleMeshVerticesBuffer);
	f->glBufferData(GL_ARRAY_BUFFER, m_triangleMeshVertices.size() * sizeof(TriangleMeshVertex), m_triangleMeshVertices.data(), GL_STATIC_DRAW);

	f->glCreateBuffers(1, &m_glTriangleMeshIndicesBuffer);
	if (m_glTriangleMeshIndicesBuffer <= 0) {
		assert(false && "Cannot create the index buffer.\n");
		std::cout << "Cannot create the index buffer." << std::endl;
		return;
	}

	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glTriangleMeshIndicesBuffer);
	f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangleMeshIndices.size() * sizeof(uint32_t), m_triangleMeshIndices.data(), GL_STATIC_DRAW);
}

void VisUtils::TriangleMesh::draw()
{
	QOpenGLFunctions_4_5_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glUseProgram(m_glTriangleMeshRenderProgram.programId());
	int stride = sizeof(TriangleMeshVertex);
	
	f->glBindBuffer(GL_ARRAY_BUFFER, m_glTriangleMeshVerticesBuffer);
	f->glVertexAttribIPointer((GLint)0, 1, GL_UNSIGNED_INT,  stride, (char*)0 + offsetof(TriangleMeshVertex, id));
	f->glVertexAttribPointer((GLint)1, 3, GL_FLOAT,			GL_FALSE, stride, (char*)0 + offsetof(TriangleMeshVertex, position));
	f->glVertexAttribPointer((GLint)2, 3, GL_FLOAT,			GL_FALSE, stride, (char*)0 + offsetof(TriangleMeshVertex, normal));
	f->glVertexAttribPointer((GLint)3, 4, GL_FLOAT,			GL_FALSE, stride, (char*)0 + offsetof(TriangleMeshVertex, result));
	
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glEnableVertexAttribArray(2);
	f->glEnableVertexAttribArray(3);

	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glTriangleMeshIndicesBuffer);

	f->glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_triangleMeshIndices.size()), GL_UNSIGNED_INT, nullptr);

	f->glDisableVertexAttribArray(0);
	f->glDisableVertexAttribArray(1);
	f->glDisableVertexAttribArray(2);
	f->glDisableVertexAttribArray(3);

}

void VisUtils::TriangleMesh::updateBuffer()
{
}

void VisUtils::TriangleMesh::freeBuffer()
{
	QOpenGLFunctions_4_5_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();

	if (m_glTriangleMeshVerticesBuffer > 0) f->glDeleteBuffers(1, &m_glTriangleMeshVerticesBuffer);	m_glTriangleMeshVerticesBuffer	= 0;
	if (m_glTriangleMeshIndicesBuffer  > 0) f->glDeleteBuffers(1, &m_glTriangleMeshIndicesBuffer);	m_glTriangleMeshIndicesBuffer	= 0;

	m_glTriangleMeshRenderProgram.release();
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
