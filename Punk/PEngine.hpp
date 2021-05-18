#pragma once
#include "PunkDefines.hpp"
#include "PWindow.hpp"
#include "PGraphics.hpp"
#include "PGameObject.hpp"
#include "TaskRunner.hpp"

class PScene;

class PunkBase {
private:
	
public:
	void Initialize(PSystemVariables settings);
	void Run();
	void Stop();

	//std::shared_ptr<PWindow> GetWindow() const { return m_GameWindow; };
protected:
	PSystemVariables m_Settings;
	//std::shared_ptr<PGameObject> sceneRoot;
	PTaskRunner ProcessRunner;
	std::shared_ptr<PScene> m_scene;
	//std::shared_ptr<PWindow> m_GameWindow;
};