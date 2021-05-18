#pragma once

#include "PunkDefines.hpp"
#include "PWindow.hpp"
#include "PGraphics.hpp"
#include "TaskRunner.hpp"

class PSystems {
private:
	static std::shared_ptr<PWindow> m_Window;
	static std::shared_ptr<PGraphics> m_Graphics;
	static std::shared_ptr<PTaskRunner> m_TaskRunner;
public:
	static void Initialize(PSystemVariables settings);
	static std::shared_ptr<PWindow> GetWindow();
	static std::shared_ptr<PGraphics> GetGraphics();
	static std::shared_ptr<PTaskRunner> GetTaskRunner();

	static void Set(const std::shared_ptr<PWindow>& window);
	static void Set(const std::shared_ptr<PGraphics>& graphics);
	static void Set(const std::shared_ptr<PTaskRunner>& taskRunner);
};