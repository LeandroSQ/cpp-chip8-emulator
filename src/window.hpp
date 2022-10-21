#pragma once
#include "input.hpp"
#include "size.hpp"
#include <SDL2/SDL.h>
#include <string>

class Window {
  private:
	SDL_Window* sdlWindowHandle = nullptr;
	SDL_Renderer* sdlRendererHandle = nullptr;
	SDL_Texture* frameBuffer = nullptr;
	void* pixels = nullptr;
	int pitch = 0;

	int8_t createWindow();
	int8_t createFrameBuffer();

  public:
	bool isClosed = false;
	int pixelScale = 10;
	Size viewport = { 64, 32 };
	Size logicalViewport = viewport * pixelScale;

	Window();

	~Window();

	int8_t init();

	void pollEvents(Input& emulator);

	void startFrame();

	void endFrame();

	void updateTitle(std::string title);

	uint8_t* getPixelsBuffer();
};