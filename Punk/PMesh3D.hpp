#pragma once
#include "PMesh.hpp"
#include "PunkDefines.hpp"
#include "PTexture.h"
#include "PVertexArrayBuffer.hpp"
#include "PIDSequence.hpp"

class PMesh3D : public PMesh 
{
	friend class PModel;
public:
	//PMesh3D(std::vector<PVertex3D> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<PTexture>> textures);
	PMesh3D(PVertex3DDataStruct vertexData, std::vector<std::shared_ptr<PTexture>> textures);
	~PMesh3D();

	void Initialize() override;

	void Render(PShader* shader) override;

private:
	PVertex3DDataStruct m_vertexData;

	static PIDSequence m_idGen;
	//std::vector<PVertex3D> _vertices;
	//std::vector<unsigned int> _indices;
	std::vector<std::shared_ptr<PTexture>> _textures;
	//unsigned int VAO, VBO, EBO;
};