#pragma once

#include "window.hpp"
#include <imgui.h>

class GUI {
  private:
	ImGuiContext* context;
	Window& window;

  public:
	GUI(Window& window);

	~GUI();

	int8_t init();

	void update();

	void render();
};