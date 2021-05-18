#pragma once

#include "PunkDefines.hpp"
#include "PTask.hpp"

class PRenderer3D;
class PGameObject;
class PCamera;
class PLight;
class PShader;
class PEditorViewportWindow;
class PScene : public Task, public std::enable_shared_from_this<PScene>
{
	friend class PEditorViewportWindow;
public:
	PScene(const std::string& name = "Untitled");
	virtual ~PScene(){}

	virtual void Initialize() override;
	virtual void Update(double deltaTime) override;
	virtual void CleanUp() override;
	virtual void Play();
	virtual void Pause() override;
	virtual void Stop();
	virtual void Finish();

	std::shared_ptr<PGameObject> Root() { return m_root; }

	std::string& Name() { return m_name; }

	PSystemVariables GameConfig() { return m_gameSettings; }

	void AddGameObject(std::shared_ptr<PGameObject> gameObject);
	std::shared_ptr<PGameObject> FindGameObject(const std::string& id);
	void RemoveGameObject(std::shared_ptr<PGameObject> gameObject);

	std::map<std::string, std::shared_ptr<PGameObject>> GameObjects() { return m_gameObjects; }

	glm::mat4 TopMatrix();
	void PushMatrix(const glm::mat4& matrix);
	void PopMatrix();

	glm::mat4& ViewProjection() { return m_viewProjection; }
	std::shared_ptr<PCamera> ActiveCamera() { return m_activeCamera; }
	std::map<std::string, std::shared_ptr<PLight>> GameLights() { return m_gameLights; }
protected:

	virtual void SetupRenderPasses();	
	void UpdateViewProjectionMatrices();
	void UpdateLightspaceMatrices();

	std::shared_ptr<PRenderer3D> m_renderer3D = nullptr;

	std::string m_name;
	PPlayState m_playState;

	std::shared_ptr<PGameObject> m_root = nullptr;
	std::shared_ptr<PCamera> m_activeCamera = nullptr;

	//temporary
	std::shared_ptr<PShader> m_shadowShader = nullptr;
	std::shared_ptr<PShader> m_depthShader = nullptr;
	std::shared_ptr<PShader> m_colorShader = nullptr;
	std::shared_ptr<PShader> m_postShader = nullptr;

	std::map<std::string,std::shared_ptr<PGameObject>> m_gameObjects;
	std::map<std::string, std::shared_ptr<PLight>> m_gameLights;

	std::list<glm::mat4> m_matrixStack;

	glm::mat4 m_viewProjection;

	PSystemVariables m_gameSettings;
};