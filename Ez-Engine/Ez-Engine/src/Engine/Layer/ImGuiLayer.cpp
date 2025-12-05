#include"Engine/Engine.h"
#include"Engine/Layer/ImGuiLayer.h"

void ImGuiLayer::Setup() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	GraphicsDevice* graphics = ENGINE::Engine::Get().Backend()->GetGraphicDevice();

	ImGui_ImplWin32_Init(ENGINE::Engine::Get().Backend()->GetHwnd());
	ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetContext());
}

void ImGuiLayer::NewFrame() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();
}

void ImGuiLayer::EndFrame() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}