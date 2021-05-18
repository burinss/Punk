#include "PEditorScene.hpp"
#include "PRenderer3D.hpp"
#include "PRenderPass.hpp"
#include "PGlobals.hpp"
#include "PGameObject.hpp"
#include "PShader.hpp"
#include "PStaticMeshComponent.hpp"

void PEditorScene::Initialize()
{
	PScene::Initialize();
	m_highlightShader=std::make_shared<PShader>(
			std::string("outline.vert"),
			std::string("outline.frag"),
			std::string("")
		);
		
	m_highlightShader->Initialize();
}

void PEditorScene::Update(double deltaTime)
{
	for (auto pass : m_renderer3D->Passes())
		pass->SetSize(m_gameSettings.windowSettings.windowSize);

	UpdateViewProjectionMatrices();
	UpdateLightspaceMatrices();
	m_renderer3D->Render(deltaTime);
}

void PEditorScene::SetupRenderPasses()
{

	std::shared_ptr<PRenderPass> shadowPass = std::make_shared<PRenderPass>(
		m_root, m_shadowShader, PRenderOp::Shadow, m_gameSettings.renderSettings.shadowMapResolution
		);
	shadowPass->SetIsSizeFixed(true);
	m_renderer3D->AddPass(shadowPass);

	std::shared_ptr<PRenderPass> colorPass = std::make_shared<PRenderPass>(
		m_root, m_colorShader, PRenderOp::Color, PSystems::GetWindow()->Resolution(),
		true, shadowPass
		);
	m_renderer3D->AddPass(colorPass);

}
