#include "PStaticMeshComponent.hpp"
#include "PGameObject.hpp"
#include "PShader.hpp"
#include "PScene.hpp"
#include "PCamera.hpp"
#include "PLight.hpp"
#include "PGlobals.hpp"
#include "PEditorScene.hpp"
#include <glm/gtc/matrix_transform.hpp>

void PStaticMeshComponent::Render(double deltaTime, const std::shared_ptr<PShader>& shader, PRenderOp renderOP )
{
	std::shared_ptr<PEditorScene> editor;
	bool selected = false;
	if (editor = std::dynamic_pointer_cast<PEditorScene>(m_object->Scene()))
		if (editor->m_selectionContext == m_object->ID())
			selected = true;

	if (auto scene = m_object->Scene()) {

		PSystems::GetGraphics()->EnableStencilBuffer();

		glm::mat4 modelMatrix = m_object->ModelMatrix();
		glm::mat4 projectionMatrix = scene->ActiveCamera()->ProjectionMatrix();
		glm::mat4 viewMatrix = scene->ActiveCamera()->ViewMatrix();
		glm::mat4 viewProjection = scene->ViewProjection();

		shader->Use();
		shader->SetMat4("view", viewMatrix);
		shader->SetMat4("projection", projectionMatrix);
		//shader->SetMat4("viewProjection", viewProjection);
		shader->SetVec3("viewPos", scene->ActiveCamera()->Position());
		shader->SetMat4("model", modelMatrix);

		int numLights = 0;
		for (auto light : scene->GameLights())
		{
			if (light.second->enabled)
			{
				shader->SetVec3(std::string("LightsArray[") + std::to_string(numLights) + std::string("]") + std::string(".lightColor"), light.second->color);
				shader->SetVec3(std::string("LightsArray[") + std::to_string(numLights) + std::string("]") + std::string(".position"), light.second->Position());
				shader->SetFloat(std::string("LightsArray[") + std::to_string(numLights) + std::string("]") + std::string(".lightRadius"), light.second->radius);
				shader->SetFloat(std::string("LightsArray[") + std::to_string(numLights) + std::string("]") + std::string(".compression"), light.second->compression);

				numLights++;
			}
		}

		shader->SetInt("numLights", numLights);

		//shader->SetVec3("lightColor", scene->GameLights().begin()->second->color);
		shader->SetMat4("P_lightSpace", scene->GameLights().begin()->second->LightSpaceMatrix());

		//shader->SetFloat("lightRadius", 100.f);
		//shader->SetFloat("compression", 5.f);

		//if(selected)PSystems::GetGraphics()->EnableStencilBuffer();

		m_mesh->Render(shader.get());

		//if (selected)PSystems::GetGraphics()->DisableStencilBuffer();
		if (selected)DrawOutlineIfEnabled(editor, modelMatrix, viewProjection);
		//PSystems::GetGraphics()->EnableStencilBuffer();
	}
}

void PStaticMeshComponent::DrawOutlineIfEnabled(const std::shared_ptr<PEditorScene> context,const glm::mat4& model, const glm::mat4& viewProjection)
{
	PSystems::GetGraphics()->DisableStencilBuffer();
	//PSystems::GetGraphics()->DisableDepthTesting();
	glm::mat4 highlightModelMatrix = glm::scale(model, glm::vec3(1.01f, 1.01f, 1.01f));
	auto highlightShader = context->m_highlightShader;
	highlightShader->Use();
	highlightShader->SetMat4("model", highlightModelMatrix);
	highlightShader->SetMat4("viewProjection", viewProjection);
	highlightShader->SetVec4("color", glm::vec4(1.0f));
	m_mesh->Render(highlightShader.get());
	PSystems::GetGraphics()->EnableStencilBuffer();
	//PSystems::GetGraphics()->EnableDepthTesting();
}
