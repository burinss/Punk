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
#include "PScene.hpp"
#include "PEditorScene.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "PEditorLayout.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#define SECONDS_TIME std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() * 0.001

void PunkBase::Initialize(PSystemVariables settings)
{

	m_Settings = settings;
    
	PSystems::Initialize(settings);
	
	PSystems::GetGraphics()->EnableFaceCulling();
	PSystems::GetGraphics()->CullBackFaces();
	PSystems::GetGraphics()->EnableDepthTesting();
	PSystems::GetGraphics()->EnableAlphaBlending();
	PSystems::GetGraphics()->EnableStencilTesting();
	PSystems::GetGraphics()->EnableMSAA();

	m_scene = std::make_shared<PEditorScene>();
	m_scene->Initialize();
	//PModel Model("room/bathroom.fbx");
	PModel Model("../../assets/models/house/WoodenCabinObj.obj");
	//PModel Model("../../assets/models/house/bathroom.fbx");
	//PModel Model("KITBASH3D_SAMPLE.fbx");
	//PModel Model("ventilator/vent.3ds");
	//PModel Model("ludi/boy.3ds");
	//PModel Model("source/scene.fbx");
	//
	PSystems::GetTaskRunner()->AttachProcess(std::static_pointer_cast<Task>(m_scene));
	for (auto tmp : Model.objects)
		m_scene->AddGameObject(tmp);

	std::shared_ptr<PCamera> camera(new PCamera);
	camera->SetName("TestCamera");
	std::shared_ptr<PLight> Light(new PLight);
	Light->SetName("TestLight");
	m_scene->AddGameObject(camera);
	m_scene->AddGameObject(Light);

	std::shared_ptr<PEditorLayout> editorLayout=std::make_shared<PEditorLayout>(m_scene);
	
	PSystems::GetTaskRunner()->AttachProcess(editorLayout);
	//PSystems::GetGraphics()->DisableMSAA();
}

void PunkBase::Run()
{
	double deltaTime_;
	double previousTime = SECONDS_TIME;
	//m_scene->Play();
	//for (std::pair<std::string, std::shared_ptr<PGameObject>> object : m_scene->GameObjects())
	//	std::cout << object.second->Name() << "\t" << object.first << std::endl;
	
	while (!PSystems::GetWindow()->IsClosing())
	{
		
		double currentTime = SECONDS_TIME;
		deltaTime_ = currentTime - previousTime;
		previousTime = currentTime;

		PSystems::GetTaskRunner()->UpdateTick(deltaTime_);
		
		PSystems::GetWindow()->SwapBuffers();

		PSystems::GetWindow()->PollEvents();
		//PSystems::Input()->PollEvents();
	}
	
}

void PunkBase::Stop()
{
}
