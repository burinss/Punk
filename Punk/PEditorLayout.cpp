#include "PEditorLayout.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <GLFW/glfw3.h>
#include "PEditorHierarchyWindow.hpp"
#include "PEditorViewportWindow.hpp"
#include "PScene.hpp"
#include "PEditorScene.hpp"
#include "PEditorPropertyWindow.hpp"
#include "PEditorMenuBar.hpp"
#include "PEditorContentBrowserWindow.hpp"
#include "PEditorToolBar.hpp"
#include "ImGuizmo.h"
#include "PTexture.h"

void PEditorLayout::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	auto& fonts = ImGui::GetIO().Fonts;
	fonts->AddFontFromFileTTF("arial.ttf", 14.0f,NULL,fonts->GetGlyphRangesCyrillic());
	fonts->Build();

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg]			=	ImVec4(0.17f, 0.16f, 0.16f, 0.85f);
	colors[ImGuiCol_ChildBg]			=	ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TitleBg]			=	ImVec4(0.09f, 0.09f, 0.09f, 0.83f);
	colors[ImGuiCol_TitleBgActive]		=	ImVec4(0.13f, 0.13f, 0.13f, 0.87f);
	colors[ImGuiCol_TitleBgCollapsed]	=	ImVec4(0.81f, 0.15f, 0.36f, 0.20f);
	colors[ImGuiCol_MenuBarBg]			=	ImVec4(0.16f, 0.16f, 0.18f, 0.80f);
	colors[ImGuiCol_Header]				=	ImVec4(0.39f, 0.39f, 0.39f, 0.45f);
	colors[ImGuiCol_HeaderHovered]		=	ImVec4(0.55f, 0.55f, 0.55f, 0.80f);
	colors[ImGuiCol_HeaderActive]		=	ImVec4(0.53f, 0.53f, 0.85f, 0.80f);
	colors[ImGuiCol_Tab]				=	ImVec4(0.13f, 0.13f, 0.13f, 0.79f);
	colors[ImGuiCol_TabHovered]			=	ImVec4(0.38f, 0.38f, 0.38f, 0.80f);
	colors[ImGuiCol_TabActive]			=	ImVec4(0.63f, 0.63f, 0.63f, 0.84f);
	colors[ImGuiCol_TabUnfocusedActive] =	ImVec4(0.11f, 0.11f, 0.11f, 0.84f);
	//colors[ImGuiCol_TableHeaderBg]		=	ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]	=	ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBg]			=	ImVec4(0.23f, 0.23f, 0.23f, 0.83f);
	colors[ImGuiCol_TableHeaderBg]		=	ImVec4(0.32f, 0.34f, 0.47f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] =   ImVec4(0.42f, 0.42f, 0.42f, 0.84f);

	auto &frameRounding =  ImGui::GetStyle().FrameRounding;
	frameRounding = 2.0f;

	ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
	ImGui_ImplOpenGL3_Init("version 400 core");

	m_windows.push_back(std::make_shared<PEditorToolBar>(shared_from_this()));
	m_windows.push_back(std::make_shared<PEditorMenuBar>(shared_from_this()));
	m_windows.push_back(std::make_shared<PEditorHierarchyWindow>(shared_from_this()));
	m_windows.push_back(std::make_shared<PEditorViewportWindow>(shared_from_this()));
	m_windows.push_back(std::make_shared<PEditorPropertyWindow>(shared_from_this()));
	m_windows.push_back(std::make_shared<PEditorContentBrowserWindow>(shared_from_this()));

	LoadIconImages();

	Task::Initialize();
}


void PEditorLayout::Update(double deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	ImGuizmo::BeginFrame();

	for (auto it : m_windows)
		it->Render();

	ImGui::EndFrame();
	ImGui::Render();
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PEditorLayout::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void PEditorLayout::LoadIconImages()
{
	namespace fs = std::filesystem;

	auto iconPath = fs::path(assetsPath + "icons");

	m_iconMap.insert({ "Folder", std::make_shared<PTexture>() });
	m_iconMap.insert({ "File", std::make_shared<PTexture>() });
	m_iconMap.insert({ "Move", std::make_shared<PTexture>() });
	m_iconMap.insert({ "Rotate", std::make_shared<PTexture>() });
	m_iconMap.insert({ "jpg", std::make_shared<PTexture>() });
	m_iconMap.insert({ "obj", std::make_shared<PTexture>() });
	m_iconMap.insert({ "fbx", std::make_shared<PTexture>() });
	m_iconMap.insert({ "png", std::make_shared<PTexture>() });
	


	m_iconMap["File"]->Load("File.png", iconPath.generic_string());
	m_iconMap["Folder"]->Load("Folder.png", iconPath.generic_string());
	m_iconMap["Move"]->Load("Move.png", iconPath.generic_string());
	m_iconMap["Rotate"]->Load("Rotate.png", iconPath.generic_string());
	m_iconMap["jpg"]->Load("Jpg.png", iconPath.generic_string());
	m_iconMap["obj"]->Load("Obj.png", iconPath.generic_string());
	m_iconMap["png"]->Load("Png.png", iconPath.generic_string());
	m_iconMap["fbx"]->Load("Fbx.png", iconPath.generic_string());
}
