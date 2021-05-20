#include "PScene.hpp"
#include "PGameObject.hpp"
#include "PComponent.hpp"
#include "PLight.hpp"
#include "PCamera.hpp"
#include "PGlobals.hpp"
#include "PRenderPass.hpp"
#include "PRenderer3D.hpp"
#include "PShader.hpp"
#include "PSceneRoot.h"

void PScene::AddGameObject(std::shared_ptr<PGameObject> gameObject)
{
	if (gameObject != nullptr)
	{
		gameObject->m_sceneContext = shared_from_this();
		if (auto camera = std::dynamic_pointer_cast<PCamera>(gameObject))
		{
			//if (camera->IsPrimary()) primaryCamera_ = camera;
			m_activeCamera = camera;
		}
		else if (auto light = std::dynamic_pointer_cast<PLight>(gameObject))
		{
			m_gameLights.insert({ gameObject->ID(), light });
		}

		/*if (auto skybox = std::dynamic_pointer_cast<ZSkybox>(gameObject))
		{
			skybox_ = skybox;
		}
		else
		{*/
			m_gameObjects.insert({ gameObject->ID(), gameObject });
		//}

		m_root->AddChild(gameObject);

		//if (runImmediately) {
			for (auto comp : gameObject->m_components)
				PSystems::GetTaskRunner()->AttachProcess(std::static_pointer_cast<Task>(comp));
		//}
	}
}

std::shared_ptr<PGameObject> PScene::FindGameObject(const std::string& id)
{
	if (!id.empty())
	{
		for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
		{
			if (it->first==id)
			{
				return it->second;
			}
			else if (it->second->HasChildren())
			{
				auto found = it->second->Child<PGameObject>(id);
				if (found) return found;
			}
		}
	}
	return nullptr;
}

void PScene::RemoveGameObject(std::shared_ptr<PGameObject> gameObject)
{
	if (gameObject != nullptr)
	{
		if (m_gameLights.find(gameObject->ID()) != m_gameLights.end())
		{
			m_gameLights.erase(gameObject->ID());
		}
		m_gameObjects.erase(gameObject->ID());
		m_root->RemoveChild(gameObject, true);
		gameObject->m_parent.reset();
		gameObject->m_sceneContext.reset();
	}
}

glm::mat4 PScene::TopMatrix()
{
	glm::mat4 M;
	if (m_matrixStack.empty()) M = glm::mat4(1.f);
	else M = m_matrixStack.back();
	return M;
}

void PScene::PushMatrix(const glm::mat4& matrix)
{
	m_matrixStack.push_back(matrix);
}

void PScene::PopMatrix()
{
	if (!m_matrixStack.empty()) m_matrixStack.pop_back();
}

void PScene::SetupRenderPasses()
{
	std::shared_ptr<PRenderPass> shadowPass = std::make_shared<PRenderPass>(
		m_root, m_shadowShader, PRenderOp::Shadow, m_gameSettings.renderSettings.shadowMapResolution
		);
	shadowPass->SetIsSizeFixed(true);
	m_renderer3D->AddPass(shadowPass);

	/*std::shared_ptr<PRenderPass> depthPass = std::make_shared<PRenderPass>(
		m_root, m_depthShader, PRenderOp::Depth, m_gameSettings.renderSettings.shadowMapResolution
		);
	depthPass->SetIsSizeFixed(true);
	m_renderer3D->AddPass(depthPass);*/

	std::shared_ptr<PRenderPass> colorPass = std::make_shared<PRenderPass>(
		m_root, m_colorShader, PRenderOp::Color, PSystems::GetWindow()->Resolution(),
		true, shadowPass
		);
	m_renderer3D->AddPass(colorPass);

	std::shared_ptr<PRenderPass> postPass = std::make_shared<PRenderPass>(
		m_root, m_postShader, PRenderOp::Post, PSystems::GetWindow()->Resolution(),
		true, colorPass
		);
	m_renderer3D->AddPass(postPass);
}

void PScene::UpdateViewProjectionMatrices()
{
	if (m_activeCamera)
	{
		m_viewProjection = m_activeCamera->ProjectionMatrix() * m_activeCamera->ViewMatrix();
	}
	else
	{
		m_viewProjection = glm::mat4(1.f);
	}
}

void PScene::UpdateLightspaceMatrices()
{
	if (m_gameLights.empty()) return;
	PFrustum frustum = m_activeCamera->Frustum();
	for (auto it = m_gameLights.begin(); it != m_gameLights.end(); it++) {
		it->second->UpdateLightspaceMatrix(frustum);
	}
}

PScene::PScene(const std::string& name):m_name(name), m_playState(PPlayState::NotStarted)
{
}

void PScene::Initialize()
{
	m_root = std::make_shared<PSceneRoot>("World");

	m_renderer3D = std::make_shared<PRenderer3D>();

	m_shadowShader	=	std::shared_ptr<PShader>(new PShader(assetsPath + std::string("shaders/shadow.vert"), assetsPath + std::string("shaders/depth.frag"), std::string("")));
	m_colorShader	=	std::shared_ptr<PShader>(new PShader(assetsPath + std::string("shaders/default.vs"), assetsPath + std::string("shaders/default.fs"), std::string("")));
	m_postShader	=	std::shared_ptr<PShader>(new PShader(assetsPath + std::string("shaders/postprocess.vert"), assetsPath + std::string("shaders/postprocess.frag"), std::string("")));

	m_shadowShader->Initialize();
	m_colorShader->Initialize();
	m_postShader->Initialize();

	SetupRenderPasses();

	Task::Initialize();
}

void PScene::Update(double deltaTime)
{
	if (m_playState != PPlayState::NotStarted)
	{
		UpdateViewProjectionMatrices();
		UpdateLightspaceMatrices();
		m_renderer3D->Render(deltaTime);
		//renderer2D_->Render(deltaTime);
	}
}

void PScene::CleanUp()
{
	for (std::map<std::string,std::shared_ptr<PGameObject>>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
		it->second->Destroy();
	m_gameObjects.clear();
}

void PScene::Play()
{
	m_playState = PPlayState::Playing;
	//gameSystems_.physics->Resume();
}

void PScene::Pause()
{
	m_playState = PPlayState::Paused;
}

void PScene::Stop()
{
	m_playState = PPlayState::NotStarted;
}

void PScene::Finish()
{
	Stop();
	CleanUp();
	Task::Finish();
}
