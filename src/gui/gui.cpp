#include "gui.hpp"

#include "../../bindings/imgui_impl_sdl.h"
#include "../app/app.hpp"
#include "../app/window.hpp"
#include "../emulator/emulator.hpp"
#include "../include/imgui_impl_sdlrenderer.h"
#include "../utils/logger.hpp"

#include <imgui.h>
#include <SDL2/SDL.h>
#include <implot.h>

#include "cpu.hpp"
#include "log.hpp"
#include "debug.hpp"
#include "style.hpp"
#include "input.hpp"
#include "memory.hpp"
#include "viewport.hpp"
#include "dissasembler.hpp"

GUI::GUI(App& app) : app(app) { }

GUI::~GUI() {
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplSDLRenderer_Shutdown();
    ImPlot::DestroyContext();
	ImGui::DestroyContext(context);
}

int8_t GUI::init() {
	// Check version
	if (!IMGUI_CHECKVERSION()) {
		Log::error("[GUI] ImGui version mismatch");
		return -1;
	}

	// Create context
	context = ImGui::CreateContext();
	if (context == nullptr) {
		Log::error("[GUI] Failed to create ImGui context");
		return -1;
	}
	ImGui::SetCurrentContext(context);

    // Setup Implot
    ImPlot::CreateContext();

	// Setup Dear ImGui style
	UI::defineStyle();

	// Setup flags
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Platform/Renderer bindings
	if (!ImGui_ImplSDL2_InitForSDLRenderer(app.window.sdlWindowHandle, app.window.sdlRendererHandle)) return -1;
	if (!ImGui_ImplSDLRenderer_Init(app.window.sdlRendererHandle)) return -1;

	return 0;
}

void GUI::pollEvent(SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void GUI::update() { }

void GUI::render() {
	ImGui_ImplSDL2_NewFrame(app.window.sdlWindowHandle);
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);

    // Render children
	UI::renderViewport(app.window);
	UI::renderCPU(app.emulator);
	UI::renderMemory(app.emulator);
    UI::renderOverlay(app);
    UI::renderDisassembler(app);
    UI::renderInput(app);
#ifdef ENABLE_LOG_BUFFER
	UI::renderLog();
#endif

	// ImGui::ShowDemoWindow();

	ImGui::Render();

	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}