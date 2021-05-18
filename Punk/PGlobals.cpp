#include "PGlobals.hpp"
#include "PWindowGL.hpp"
#include "PGraphicsGL.hpp"

std::shared_ptr<PWindow> PSystems::m_Window = nullptr;
std::shared_ptr<PGraphics> PSystems::m_Graphics = nullptr;
std::shared_ptr<PTaskRunner> PSystems::m_TaskRunner = nullptr;

void PSystems::Initialize(PSystemVariables settings)
{
    Set(std::make_shared<PWindowGL>(settings.windowSettings));
    Set(std::make_shared<PGraphicsGL>());
    Set(std::make_shared<PTaskRunner>());
}

std::shared_ptr<PWindow> PSystems::GetWindow()
{
    return m_Window;
}

std::shared_ptr<PGraphics> PSystems::GetGraphics()
{
    return m_Graphics;
}

std::shared_ptr<PTaskRunner> PSystems::GetTaskRunner()
{
    return m_TaskRunner;
}

void PSystems::Set(const std::shared_ptr<PWindow>& window)
{
    m_Window = window;
    m_Window->Initialize();
}

void PSystems::Set(const std::shared_ptr<PGraphics>& graphics)
{
    m_Graphics = graphics;
    m_Graphics->Initialize();
}

void PSystems::Set(const std::shared_ptr<PTaskRunner>& taskRunner)
{
	m_TaskRunner = taskRunner;
    //m_TaskRunner->Initalize();
}