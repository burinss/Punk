#include "PWindowGL.hpp"
#include "PGlobals.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


void PWindowGL::Initialize()
{
	if (!glfwInit()) {
		std::cout << "Failed to initialize glfw" << std::endl;
		return;
	}
	m_WindowHandle = CreateWindow(m_WindowSettings.windowSize.x, m_WindowSettings.windowSize.y, m_WindowSettings.maximized);

	glfwSetWindowUserPointer((GLFWwindow*)m_WindowHandle, this);
	glfwSetFramebufferSizeCallback((GLFWwindow*)m_WindowHandle, FrameBufferResizeCallback);
	//glfwSetWindowSizeCallback(window, WindowSizeCallback);
	m_WindowSettings.resolution = m_WindowSettings.windowSize;
}

void PWindowGL::PollEvents()
{
	glfwPollEvents();
}

void PWindowGL::Close()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(m_WindowHandle);
	glfwSetWindowShouldClose(window, true);
}

bool PWindowGL::IsClosing()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(m_WindowHandle);
	return glfwWindowShouldClose(window);
}

void PWindowGL::SwapBuffers()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(m_WindowHandle);
	glfwSwapBuffers(window);
}

void PWindowGL::FramebufferResize(int width, int height)
{
	this->m_WindowSettings.resolution.x = (float)width; this->m_WindowSettings.resolution.y = (float)height;
 }

void PWindowGL::Destroy()
{
	glfwDestroyWindow((GLFWwindow*)m_WindowHandle);
}


void PWindowGL::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	//PSystems::GetGraphics()->UpdateViewport(glm::vec2(width, height), glm::vec2(0.f));
	PWindowGL* domainStrategyPtr = static_cast<PWindowGL*>(glfwGetWindowUserPointer(window));
	if (domainStrategyPtr) {
		domainStrategyPtr->FramebufferResize(width, height);
	}
}

void* PWindowGL::CreateWindow(int width, int height, bool maximized, bool visible, void* sharedContext)
{

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWwindow* windowHandle = glfwCreateWindow(width, width, "Engine", maximized ? glfwGetPrimaryMonitor() : NULL, (GLFWwindow*)sharedContext);
	if (windowHandle == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(windowHandle);
	//glfwSetFramebufferSizeCallback(windowHandle, FrameBufferResizeCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return nullptr;
	}
	
	glfwSwapInterval(1);
	return windowHandle;
}
