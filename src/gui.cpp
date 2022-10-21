#include "gui.hpp"
#include "../bindings/imgui_impl_sdl.h"
#include "../bindings/imgui_impl_sdlrenderer.h"
#include "logger.hpp"

GUI::GUI(Window& window) : window(window) { }

GUI::~GUI() {
	ImGui_ImplSDLRenderer_Shutdown();
	// ImGui_ImplSDL_Shutdown();
	ImGui::DestroyContext(context);
}

int8_t GUI::init() {
	if (!IMGUI_CHECKVERSION()) {
		Log::error("[GUI] ImGui version mismatch");
		return -1;
	}

	context = ImGui::CreateContext();
	if (context == nullptr) {
		Log::error("[GUI] Failed to create ImGui context");
		return -1;
	}
	ImGui::SetCurrentContext(context);

	// Setup Platform/Renderer bindings
	// if (!ImGui_ImplSDL2_InitForSDLRenderer(window., renderer)) return -1;
	// if (!ImGui_ImplSDLRenderer_Init(renderer)) return -1;

	return 0;
}

void GUI::update() { }

void GUI::render() { }
