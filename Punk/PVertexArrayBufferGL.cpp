#include "PVertexArrayBufferGL.hpp"
#include <glad/glad.h>

void PVertexArrayBufferGL::Bind()
{
	glBindVertexArray(m_vertexArrayObject);
	if (m_elementsBufferObject != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementsBufferObject);
	}
}

void PVertexArrayBufferGL::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PVertexArrayBufferGL::Load(const PVertex3DDataStruct& vertexData)
{
	// create buffers/arrays
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);
	glGenBuffers(1, &m_elementsBufferObject);

	glBindVertexArray(m_vertexArrayObject);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertexData.vertices.size() * sizeof(PVertex3D), &vertexData.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementsBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexData.indices.size() * sizeof(unsigned int), &vertexData.indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PVertex3D), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PVertex3D), (void*)offsetof(PVertex3D, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PVertex3D), (void*)offsetof(PVertex3D, uv));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(PVertex3D), (void*)offsetof(PVertex3D, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(PVertex3D), (void*)offsetof(PVertex3D, bitangent));

	glBindVertexArray(0);
}

void PVertexArrayBufferGL::Load(const PVertex2DDataStruct& vertexData)
{
	// create buffers/arrays
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);
	//glGenBuffers(1, &m_elementsBufferObject);

	glBindVertexArray(m_vertexArrayObject);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertexData.vertices.size() * sizeof(PVertex2D), &vertexData.vertices[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementsBufferObject);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexData.indices.size() * sizeof(unsigned int), &vertexData.indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PVertex2D), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex texture coords
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PVertex2D), (void*)offsetof(PVertex2D, uv));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PVertexArrayBufferGL::Delete()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteBuffers(1, &m_elementsBufferObject);
}
