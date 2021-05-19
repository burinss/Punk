#include "PIconComponent.hpp"
#include "PMesh2D.hpp"
#include "PTexture.h"
#include "PShader.hpp"
#include "PGameObject.hpp"
#include "PScene.hpp"

void PIconComponent::Initialize()
{
	PComponent::Initialize();
	m_mesh = PMesh2D::NewQuad();
	m_iconShader = std::make_shared<PShader>("../../assets/shaders/billboard.vert", "../../assets/shaders/billboard.frag","");
	m_iconShader->Initialize();
	m_icon = std::make_shared<PTexture>();
	m_icon->Load("light.png", "../../assets/icons/");
}

void PIconComponent::Render(double deltaTime, const std::shared_ptr<PShader>& shader)
{
	if (m_iconShader!=nullptr)
	{
		m_iconShader->Use();
		m_icon->Bind(2);
		m_iconShader->SetInt("billboardTexture", 2);
		m_iconShader->SetVec2("billboardSize", glm::vec2(0.1f));
		m_iconShader->SetVec3("billboardPosition", m_object->Position());
		//m_iconShader->SetMat4("model", m_object->ModelMatrix());
		m_iconShader->SetMat4("viewProjection", m_object->Scene()->ViewProjection());
		m_mesh->Render(m_iconShader.get());
	}
}
