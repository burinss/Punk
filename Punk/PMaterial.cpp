#include "PMaterial.hpp"
#include "PShader.hpp"
#include "PTexture.h"

PIDSequence PMaterial::m_idGen("material");

void PMaterial::Initialize()
{
	auto defaultTexture = std::make_shared<PTexture>(glm::vec4(0.5f,0.5f,0.5f,1.0f));
	m_textureSlots.insert({ "diffuse",defaultTexture });
	m_textureSlots.insert({ "normal",defaultTexture });
	m_textureSlots.insert({ "specular",defaultTexture });

	Task::Initialize();

	id_ = m_idGen.Next();
}

void PMaterial::Use(std::shared_ptr<PShader> shader)
{
	if (shader == nullptr)
		return;

	shader->Use();

	int textureSlot = 1;

	for (auto slot : m_textureSlots)
	{
		if (slot.second != nullptr)
		{
			slot.second->Bind(textureSlot);
			shader->SetInt(slot.first, textureSlot);
			textureSlot++;
		}
	}
}

void PMaterial::Set(std::string slot, std::shared_ptr<PTexture> texture)
{
	if (texture==nullptr)
	{
		return;
	}
	m_textureSlots[slot] == texture;
}

std::shared_ptr<PTexture> PMaterial::Get(std::string slot)
{
	return m_textureSlots[slot];
}
