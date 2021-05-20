#pragma once
#include "PMesh.hpp"
#include "PunkDefines.hpp"
#include "PTexture.h"
#include "PVertexArrayBuffer.hpp"
#include "PIDSequence.hpp"

class PMaterial;

class PMesh3D : public PMesh 
{
	friend class PModel;
public:
	//PMesh3D(std::vector<PVertex3D> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<PTexture>> textures);
	PMesh3D(PVertex3DDataStruct vertexData, std::shared_ptr<PMaterial> material);
	~PMesh3D();

	void Initialize() override;

	void Render(std::shared_ptr <PShader> shader) override;

	std::shared_ptr<PMaterial>& GetMaterial() { return m_material; }
	void SetMaterial(std::shared_ptr<PMaterial> mat) { m_material = mat; }
private:
	PVertex3DDataStruct m_vertexData;

	static PIDSequence m_idGen;
	//std::vector<PVertex3D> _vertices;
	//std::vector<unsigned int> _indices;
	std::shared_ptr<PMaterial> m_material;
	//unsigned int VAO, VBO, EBO;
};