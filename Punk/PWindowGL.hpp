#pragma once

#include "PWindow.hpp"
struct GLFWwindow;

class PWindowGL : public PWindow{
private:
	static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);
public:
	PWindowGL(PWindowSettings settings) :PWindow(settings) {};
	~PWindowGL() {};

	void Initialize() override;
	void PollEvents() override;
	void Close() override;
	void Destroy() override;
	bool IsClosing() override;
	void SwapBuffers() override;

	void FramebufferResize(int width, int height) override;

	void *CreateWindow(int width, int height, bool maximized = true, bool visible = true, void* sharedContext = nullptr) override;
};