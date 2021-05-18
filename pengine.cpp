#include "PEngine.hpp"
#include "PGlobals.hpp"
#include "PWindowGL.hpp"
#include "PGraphicsGL.hpp"
#include "PModel.hpp"
#include "PShader.hpp"
#include "PGameObject.hpp"
#include "PCamera.hpp"
#include "PLight.hpp"
#include "PFramebufferGL.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>


#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
void PunkBase::Initialize(PSystemVariables settings)
{

	m_Settings = settings;
    
	PSystems::Initalize(settings);
	//PSystems::GetWindow()->Initalize();
	//PSystems::GetWindow()->CreateWindow(1366, 768, false, true, NULL);
	//PSystems::GetGraphics()->Initalize();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("version 400 core");
	ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);

	PSystems::GetGraphics()->EnableFaceCulling();
	PSystems::GetGraphics()->CullBackFaces();
	PSystems::GetGraphics()->EnableDepthTesting();
	PSystems::GetGraphics()->EnableAlphaBlending();
	PSystems::GetGraphics()->EnableMSAA();

	sceneRoot = std::make_shared<PGameObject>("World");
	//PModel Model("room/bathroom.obj");
	PModel Model("house/WoodenCabinObj.obj");
	for(auto tmp: Model.objects)
	sceneRoot->AddChild(tmp);

	//PSystems::GetGraphics()->DisableMSAA();
}

void PunkBase::Run()
{
	//PModel Model("source/scene.FBX");
	//PModel Model("house/WoodenCabinObj.obj");
	//PModel Model("room/room.obj");
	//PModel Model("kreslo01/1.obj");
	std::shared_ptr<PFramebuffer> ShadowPass;
	std::shared_ptr<PFramebuffer> ColorPass;
	ColorPass = PFramebufferGL::CreateColor(glm::vec2(1366.f, 768.f));
	ShadowPass = PFramebufferGL::CreateDepth(glm::vec2(4096.f, 4096.f));
	std::shared_ptr<PShader> ColorShader =  std::make_shared<PShader>(std::string("default.vs"), std::string("default.fs"), std::string(""));
	std::shared_ptr<PShader> ShadowShader = std::make_shared<PShader>(std::string("shadow.vert"), std::string("depth.frag"), std::string(""));
	
	//PModel Model("house/WoodenCabinObj.obj");

	glm::vec3 lightspos[] = {
		glm::vec3(-7.0f, 10.f, -20.0f),
		glm::vec3(7.0f, 10.f, -20.0f),
		glm::vec3(-7.0f, 10.f, 5.0f),
		glm::vec3(7.0f, 10.f, 5.0f)
	};

	PCamera* camera;
	camera = new PCamera();//PCamera camera(glm::vec3(0.f));

	PLight lights[] = {
		PLight(lightspos[0]/*, glm::quat(glm::vec3(0.f)),PLightType::Directional*/),
		PLight(lightspos[1]),
		PLight(lightspos[2]),
		PLight(lightspos[3])
	};
	
	//Model.Init();

	std::cout << "color compiling" << std::endl;
	ColorShader->Init();
	std::cout << "shadow compiling" << std::endl;
	ShadowShader->Init();
	/*std::vector<std::string> meshes;
	std::map<std::string, PMesh3D>::iterator it;
	for (it = Model._meshes.begin(); it != Model._meshes.end(); it++)
	{
		meshes.push_back(it->first);
	}*/
	//PFramebufferGL fb;
	//fb.LoadColor(glm::vec2(800, 600), PTexture::CreateColor(glm::vec2(800, 600)));

	float posx = 0, posy = 0, posz = 0, rotx = 0, roty = 0, rotz = 0,lightRadius=1,lightCompression=0.1;
	while (!PSystems::GetWindow()->IsClosing())
	{
		bool selected;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		//ImGui::DockSpaceOverViewport();
		ImGui::NewFrame();


		{
			ImGui::BeginMainMenuBar();
			ImGui::MenuItem("Test", "item", &selected);
			ImGui::EndMainMenuBar();
		}
		{
			ImGui::Begin("Test");


			ImGui::SliderFloat("testX", &posx, -50.f, 50.f);
			ImGui::SliderFloat("testY", &posy, -50.f, 50.f);
			ImGui::SliderFloat("testZ", &posz, -50.f, 50.f);
			ImGui::Separator();

			ImGui::SliderFloat("rotX", &rotx, -3.5f, 3.5f);
			ImGui::SliderFloat("rotY", &roty, -3.5f, 3.5f);
			ImGui::SliderFloat("rotZ", &rotz, -3.5f, 3.5f);
			ImGui::Separator();

			ImGui::SliderFloat("light1X", &lightspos[0].x, -150.f, 150.f);
			ImGui::SliderFloat("light1Y", &lightspos[0].y, -150.f, 150.f);
			ImGui::SliderFloat("light1Z", &lightspos[0].z, -150.f, 150.f);

			ImGui::Separator();
			ImGui::SliderFloat("light2X", &lightspos[1].x, -50.f, 50.f);
			ImGui::SliderFloat("light2Y", &lightspos[1].y, -50.f, 50.f);
			ImGui::SliderFloat("light2Z", &lightspos[1].z, -50.f, 50.f);

			ImGui::Separator();
			ImGui::SliderFloat("light3X", &lightspos[2].x, -50.f, 50.f);
			ImGui::SliderFloat("light3Y", &lightspos[2].y, -50.f, 50.f);
			ImGui::SliderFloat("light3Z", &lightspos[2].z, -50.f, 50.f);

			ImGui::Separator();
			ImGui::SliderFloat("light4X", &lightspos[3].x, -50.f, 50.f);
			ImGui::SliderFloat("light4Y", &lightspos[3].y, -50.f, 50.f);
			ImGui::SliderFloat("light4Z", &lightspos[3].z, -50.f, 50.f);

			ImGui::Separator();

			ImGui::SliderFloat("lightRadius", &lightRadius, 0, 150.f);
			ImGui::SliderFloat("lightCompression", &lightCompression, 0.01f, 20.f);

			/*ImGui::BeginTable("meshes", 1);
			for(auto name :meshes)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(name.c_str());
			}

			ImGui::EndTable();*/
			ImGui::End();
		}


		for (int i = 0; i < 4; i++)
		{
			lights[i].SetPosition(lightspos[i]);
		}
		camera->SetPosition(glm::vec3(posx, posy, posz));
		//camera->SetOrientation(glm::vec3(rotx, roty, rotz));
		lights[0].SetOrientation(glm::vec3(rotx, roty, rotz));
		camera->Render(ColorShader.get(),0.1f);

		
		ShadowShader->Use();
		ShadowShader->SetMat4("P_lightSpace", lights[0].LightSpaceMatrix());
		ShadowShader->SetMat4("model", glm::translate(glm::mat4(), glm::vec3(0.f)));

		ShadowPass->Bind();
		PSystems::GetGraphics()->UpdateViewport(ShadowPass->Size(), glm::vec2(0.0f));
		PSystems::GetGraphics()->ClearViewport();

		//Model.Draw(&ShadowShader);
		sceneRoot->RenderChildren(0, ShadowShader);
		ShadowPass->Unbind();

		ColorShader->Use();
		ShadowPass->Attachment()->Bind(0);
		ColorShader->SetInt("shadowTexture", 0);

		ColorShader->SetInt("numLights", 1);

		for (int i = 0; i < 4; i++)
		{
			ColorShader->SetVec3(std::string("lightsPos[")+std::to_string(i)+std::string("]"), lights[i].Position());
		}
		
		ColorShader->SetVec3("viewPos", camera->Position());
		lights[0].UpdateLightspaceMatrix(camera->Frustum());
		ColorShader->SetMat4("P_lightSpace", lights[0].LightSpaceMatrix());

		ColorShader->SetVec3("lightColor", glm::vec3(1.f));

		ColorShader->SetFloat("lightRadius", lightRadius);
		ColorShader->SetFloat("compression", lightCompression);

		ColorShader->SetMat4("view", camera->ViewMatrix());
		ColorShader->SetMat4("projection", camera->ProjectionMatrix());
		//ColorShader->SetMat4("model", glm::translate(glm::mat4(),glm::vec3(0.f)));

		ColorPass->Bind();
		PSystems::GetGraphics()->UpdateViewport(ColorPass->Size(), glm::vec2(0.0f));
		PSystems::GetGraphics()->ClearViewport();

		sceneRoot->RenderChildren(0, ColorShader);
		//Model.Draw(&ColorShader);
		ColorPass->Unbind();
		ImGui::Begin("scene");
		//fb.Unbind();
		ImVec2 size = ImGui::GetContentRegionAvail();
		/*ImGui::GetWindowDrawList()->AddImage((void*)ColorPass->Attachment()->id,
			pos,
			ImVec2(pos.x + 512, pos.y + 512),
			ImVec2(0, 1),
			ImVec2(1, 0));
			*/
		ColorPass->Resize((int)size.x, (int)size.y);
		auto frame = ColorPass->Attachment()->id;
		ImGui::Image((void*)frame, size, ImVec2(0, 1),ImVec2(1, 0));
		//glViewport(0, 0, size.x, size.y);
		ImGui::End();

		ImGui::Begin("ShadowBuffer");
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddImage((void*)ShadowPass->Attachment()->id,
			pos,
			ImVec2(pos.x + 256, pos.y + 256),
			ImVec2(0, 1),
			ImVec2(1, 0));
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//PSystems::GetGraphics()->SwapBuffers();
		PSystems::GetWindow()->SwapBuffers();
		PSystems::GetWindow()->PollEvents();
	}
}

void PunkBase::Stop()
{
}
