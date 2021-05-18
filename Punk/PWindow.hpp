#pragma once
#include "PunkDefines.hpp"



class PWindow {
private:
public:
    PWindow(PWindowSettings settings) : m_WindowSettings(settings) {};
    virtual ~PWindow() {};


	glm::vec2 WindowSize() const { return m_WindowSettings.windowSize; }
	glm::vec2 Resolution() const { return m_WindowSettings.resolution; }

    double Aspect() const { return m_WindowSettings.resolution.x / m_WindowSettings.resolution.y; }

    virtual void Initialize() = 0;
    virtual void PollEvents() = 0;
    virtual void Close() = 0;
    virtual void Destroy() = 0;
    virtual bool IsClosing() = 0;
    virtual void SwapBuffers() = 0;

    virtual void FramebufferResize(int width, int height) = 0;

    virtual void* CreateWindow(int width, int height, bool maximized = true, bool visible = true, void* sharedContext = nullptr) = 0;
protected:
    void* m_WindowHandle = nullptr;
    PWindowSettings m_WindowSettings;
};