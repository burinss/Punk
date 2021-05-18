#pragma once

#include "PunkDefines.hpp"
#include "PMesh.hpp"
class PTexture;

class PMesh2D : public PMesh
{
public:
	//PMesh2D() {}
	PMesh2D(PVertex2DDataStruct vertexData);
	~PMesh2D() {};

	void Initialize() override;

	void Render(PShader* shader) override;

	static std::shared_ptr<PMesh2D> NewQuad();
private:
	PVertex2DDataStruct m_vertexData;

	//std::vector<std::shared_ptr<PTexture>> _textures;
};