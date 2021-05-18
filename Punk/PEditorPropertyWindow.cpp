#include "PunkDefines.hpp"

#include "PEditorPropertyWindow.hpp"
#include "imgui.h"
#include "ImGuizmo.h"
#include "PEditorScene.hpp"
#include "PGameObject.hpp"
#include "PCamera.hpp"
#include "PLight.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void PEditorPropertyWindow::Render()
{
	
	ImGui::ShowStyleEditor();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Properties"))
	{
		auto editor = std::static_pointer_cast<PEditorScene>(m_layoutContext->GetContext());
		if (editor->m_selectionContext != "0")
		{
			if (auto object = editor->FindGameObject(editor->m_selectionContext)) {
				char name[256];
				strcpy_s(name, object->Name().c_str());
				if (ImGui::InputText("Name", name, 256))
				{
					object->SetName(name);
				}

				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

				ImGui::Separator();

				if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Separator();
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive));

					//ImGui::BeginChild("properties", ImVec2(0, 100.f), false, ImGuiWindowFlags_AlwaysAutoResize);

					ImGui::Text("");

					glm::vec3 position = object->Position();
					glm::quat rotation = object->Orientation();
					glm::vec3 scale = object->Scale();

					ImGui::Text("");
					ImGui::SameLine();

					if (ImGui::DragFloat3("Position", &position.x, 1.0f, 0.0f, 0.0f, "%.2f"))
						object->SetPosition(position);

					rotation = glm::normalize(rotation);
					glm::vec3 rotationVec = eulerAngles(rotation);
					glm::vec3 degreesVec = glm::degrees(rotationVec);

					ImGui::Text("");
					ImGui::SameLine();

					if (ImGui::DragFloat3("Orientation", &degreesVec.x, 1.0f, 0.0f, 0.0f, "%.2f"))
						//if (glm::distance(rotateDt, glm::vec3(0.0f)) > 0)
					{
						glm::vec3 rotateDt = glm::degrees(rotationVec) - degreesVec;
						rotation = glm::rotate(rotation, glm::radians(rotateDt.x), glm::vec3(1.0f, 0.0f, 0.0f));
						rotation = glm::rotate(rotation, glm::radians(rotateDt.y), glm::vec3(0.0f, 1.0f, 0.0f));
						rotation = glm::rotate(rotation, glm::radians(rotateDt.z), glm::vec3(0.0f, 0.0f, 1.0f));
						object->SetOrientation(rotation);
					}

					ImGui::Text("");
					ImGui::SameLine();

					if (ImGui::DragFloat3("Scale", &scale.x, 0.1f, 0.0f, 0.0f, "%.2f"))
						object->SetScale(scale);


					//ImGui::EndChild();
					ImGui::TreePop();
					ImGui::PopStyleColor(1);
				}
				ImGui::PopStyleVar(1);


				if(ImGui::Begin("Viewport"))
				{
					auto viewportPos = ImGui::GetWindowPos();
					auto viewportSize = ImGui::GetWindowSize();
					auto camera = editor->ActiveCamera();

					//glm::mat4 viewMatrix = glm::translate(glm::mat4(1.f), camera->Position()) * glm::mat4_cast(camera->Orientation());
					//auto view = glm::inverse(editor->ActiveCamera()->ModelMatrix());
					//auto view = glm::inverse(viewMatrix);
					auto view = editor->ActiveCamera()->ViewMatrix();

					glm::mat4 modelMatrix = object->ModelMatrix();

					ImGuizmo::SetDrawlist();
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

					ImGuizmo::Manipulate(&view[0][0], &editor->ActiveCamera()->ProjectionMatrix()[0][0], ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, &modelMatrix[0][0]);
				
					if (ImGuizmo::IsUsing())
					{
						glm::vec3 position;
						ImGuizmo::DecomposeMatrixToComponents(&modelMatrix[0][0], &position[0], new float[3], new float[3]);
						object->SetPosition(position);
					}
					ImGui::End();
				}
				ShowObjectProperties(object);
			}
		}
		ImGui::PopStyleVar(1);
		ImGui::End();
	}
}

void PEditorPropertyWindow::ShowObjectProperties(const std::shared_ptr<PGameObject>& gameObject)
{
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
	auto objectType = DetectObjectType(gameObject);
	if (ImGui::TreeNodeEx("General", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
		//ImGui::BeginChild("General properties", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text(typeid(*gameObject.get()).name());
		if (objectType == PEditorObjectType::LIGHT)
		{
			const std::string lightTypes[] = { "Directional", "Point", "Spot", "Area" };
			auto light = std::static_pointer_cast<PLight>(gameObject);

			ImGui::Text("Enable:");
			ImGui::SameLine(110.0f);
			ImGui::Checkbox("##enableCheckbox", &light->enabled);

			//ImGui::SameLine(70.0f);

			ImGui::Text("Type:");

			ImGui::SameLine(110.0f);

			ImGui::PushID("TypeCombo");

			ImGui::SetNextItemWidth(90.f);
			if (ImGui::BeginCombo("", lightTypes[(int)light->type].c_str()))
			{
				for (int i = 0; i < 4; i++)
				{
					if (ImGui::Selectable(lightTypes[i].c_str(), lightTypes[i] == lightTypes[(int)light->type]))
					{
						light->type = (PLightType)i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopID();

			ImGui::PushID("LightColorPicker");
			{
				//ImGui::Set
				ImGui::Text("Color:");
				ImGui::SameLine(110.0f);
				ImGui::ColorEdit3("", &light->color[0],ImGuiColorEditFlags_NoInputs);
			}
			ImGui::PopID();
		}
		else if (objectType == PEditorObjectType::CAMERA)
		{
			auto camera = std::static_pointer_cast<PCamera>(gameObject);
			ImGui::Text("Fov:");
			float zoom = camera->Zoom();
			ImGui::DragFloat("", &zoom);
			camera->SetZoom(zoom);
		}
		//ImGui::EndChild();
		ImGui::TreePop();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);	
	}
	ImGui::PopStyleVar(1);
}

PEditorObjectType PEditorPropertyWindow::DetectObjectType(const std::shared_ptr<PGameObject>& gameObject)
{
	std::string typeName = typeid(*gameObject.get()).name();
	
	if (typeName == "class PCamera")
	{
		return PEditorObjectType::CAMERA;
	}
	else if (typeName == "class PLight")
	{
		return PEditorObjectType::LIGHT;
	}
	else if (typeName == "class PGameObject")
	{
		return PEditorObjectType::OBJECT;
	}
	else
		return PEditorObjectType::UNDEFINED;
}
