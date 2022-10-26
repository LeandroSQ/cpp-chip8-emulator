#pragma once

#include "../app/window.hpp"
#include <SDL2/SDL.h>
#include <imgui.h>

// App's Forward declaration, for circular dependency resolution
class App;

class GUI {
  private:
	ImGuiContext* context;
	App& app;

  public:
	GUI(App& app);

	~GUI();

	int8_t init();

    void pollEvent(SDL_Event& event);

	void update();

	void render();
};