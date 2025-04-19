#include "GUI.h"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_win32.h"
#include "../../lib/imgui/imgui_impl_dx11.h"
#include "../Rendering/Rendering.h"

namespace Koban {
	GUI::GUI(HWND& hwnd){
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(hwnd);
	}

	GUI::~GUI() {
	}

	void GUI::start() {
		ImGui_ImplDX11_Init(RENDER->getDevice(), RENDER->getDeviceContext());
	}

	void GUI::draw() {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::End();

		// Rendering
		ImGui::Render();
	}

	void GUI::destroy() {
		//// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}