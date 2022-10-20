#pragma once

#include "../bindings/imgui_impl_sdl.h"
#include "../bindings/imgui_impl_sdlrenderer.h"
#include "logger.hpp"
#include "window.hpp"
#include <SDL2/SDL.h>

class Renderer {
  public:
	Renderer() { }

	~Renderer() { }

	void render(Window& window) {
        uint8_t* pixels = window.getPixelsBuffer();

		for (uint8_t y = 0; y < window.viewport.height; y++) {
			for (uint8_t x = 0; x < window.viewport.width; x++) {
                if (x < window.viewport.width / 2) {
                    setPixel(window, pixels, x, y, 0xFF);
                } else {
                    setPixel(window, pixels, x, y, 0x00);
                }
			}
		}
	}

    void setPixel(const Window& window, uint8_t* pixels, int x, int y, uint8_t color) {
        for (int scaleX = 0; scaleX < window.pixelScale; scaleX++) {
            for (int scaleY = 0; scaleY < window.pixelScale; scaleY++) {
                int realX = x * window.pixelScale + scaleX;
                int realY = y * window.pixelScale + scaleY;
                int totalOffset = (realX + realY * window.logicalViewport.width) * (sizeof(uint8_t) * 4);

                #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    // ARGB
                    pixels[totalOffset + 0] = 0xFF;
                    pixels[totalOffset + 1] = color;
                    pixels[totalOffset + 2] = color;
                    pixels[totalOffset + 3] = color;
                #else
                    // BGRA
                    pixels[totalOffset + 0] = color;
                    pixels[totalOffset + 1] = color;
                    pixels[totalOffset + 2] = color;
                    pixels[totalOffset + 3] = 0xFF;
                #endif
            }
        }
    }
};