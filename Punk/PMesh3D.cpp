#include <string>
#include <glad/glad.h>
#include "PGlobals.hpp"
#include "PShader.hpp"
#include "PMesh3D.hpp"
#include "PVertexArrayBuffer.hpp"

//PMesh3D::PMesh3D(std::vector<PVertex3D> vertices,
//	std::vector<unsigned int> indices,
//	std::vector<std::shared_ptr<PTexture>> textures):
//	_vertices(vertices),
//	_indices(indices),
//	_textures(textures)
//{
//	Initialize();
//}
PIDSequence PMesh3D::m_idGen("mesh");

PMesh3D::PMesh3D(PVertex3DDataStruct vertexData, std::vector<std::shared_ptr<PTexture>> textures)
	:m_vertexData(vertexData),_textures(textures)
{
	id_ = m_idGen.Next();
	Initialize();
}

PMesh3D::~PMesh3D()
{
	//_vertices.clear();
	//_indices.clear();
	//_textures.clear();
}

void PMesh3D::Initialize()
{
	m_vertexBuffer = PVertexArrayBuffer::Create(m_vertexData);

}

void PMesh3D::Render(PShader* shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = _textures[i]->type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader->GetID(), (name + number).c_str()), i+1);
		// and finally bind the texture
		_textures[i]->Bind(i+1);
	}
	
	//PSystems::GetGraphics()->Draw(VAO,_indices,PMeshDrawStyle::Triangle);
	PSystems::GetGraphics()->Draw(m_vertexBuffer,m_vertexData, PMeshDrawStyle::Triangle);
	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}
