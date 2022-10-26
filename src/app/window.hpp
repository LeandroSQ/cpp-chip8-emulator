#pragma once

#include "../emulator/input.hpp"
#include "../utils/size.hpp"
#include <SDL2/SDL.h>
#include <string>

#define ENABLE_HIGHDPI

class Window {
  private:
	void* pixels = nullptr;
	int pitch = 0;

	int8_t createWindow();
	int8_t createFrameBuffer();

  public:
	SDL_Texture* frameBuffer = nullptr;
    SDL_Window* sdlWindowHandle = nullptr;
	SDL_Renderer* sdlRendererHandle = nullptr;

	bool isClosed = false;
	int pixelScale = 10;
	Size viewport = { 64, 32 };
	Size logicalViewport = viewport * pixelScale;

	Window();

	~Window();

	int8_t init();

	void startFrame();

	void endFrame();

    void showFrame();

	void updateTitle(std::string title);

	uint8_t* getPixelsBuffer();
};