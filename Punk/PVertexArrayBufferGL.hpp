#pragma once
#include "PVertexArrayBuffer.hpp"

class PVertexArrayBufferGL : public PVertexArrayBuffer
{
public:
	~PVertexArrayBufferGL() {};


	void Bind() override;
	void Unbind() override;
	void Load(const PVertex3DDataStruct& vertexData) override;
	void Load(const PVertex2DDataStruct& vertexData) override;
	void Delete() override;
};