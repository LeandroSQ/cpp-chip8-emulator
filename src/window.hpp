#pragma once
#include <SDL2/SDL.h>
#include "size.hpp"

class Window {
  private:
    SDL_Window* sdlWindowHandle = nullptr;
	SDL_Renderer* sdlRendererHandle = nullptr;
    SDL_Texture* frameBuffer = nullptr;
	void* pixels = nullptr;
	int pitch = 0;

    uint8_t createWindow();
    uint8_t createFrameBuffer();

  public:
    bool isClosed = false;
    int pixelScale = 2;
    Size viewport = { 64, 32 };
    Size logicalViewport = viewport * pixelScale;

	Window();

	~Window();

    uint8_t init ();

    void pollEvents();

	void startFrame();

	void endFrame();

    uint8_t* getPixelsBuffer();

};