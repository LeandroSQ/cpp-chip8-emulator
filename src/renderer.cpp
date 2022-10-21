#include "renderer.hpp"
#include "../bindings/imgui_impl_sdl.h"
#include "../bindings/imgui_impl_sdlrenderer.h"
#include "logger.hpp"
#include <SDL2/SDL.h>

Renderer::Renderer(Settings& settings) : settings(settings) { }

Renderer::~Renderer() { }

void Renderer::render(Window& window) {
	uint8_t* pixels = window.getPixelsBuffer();

	// Copy the video buffer to the frame buffer texture
	for (uint8_t y = 0; y < window.viewport.height; y++) {
		for (uint8_t x = 0; x < window.viewport.width; x++) {
			uint16_t index = (y * window.viewport.width) + x;
			uint8_t currentColor = videoBuffer[index];
			uint8_t& bufferColor = lastFrameBuffer[index];

            if (!settings.isFrameInterpolationEnabled) bufferColor = currentColor;
            else if (currentColor == 0xFF) bufferColor = 0xFF;

            setPixel(window, pixels, x, y, bufferColor);

            if (settings.isFrameInterpolationEnabled && currentColor != 0xFF) {
                constexpr uint8_t step = 0x80;
                if (bufferColor > step) {
                    bufferColor -= step;
                } else {
                    bufferColor = 0;
                }
            }
		}
	}
}

void Renderer::setPixel(const Window& window, uint8_t* pixels, uint16_t x, uint16_t y, uint8_t color) {
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

void Renderer::fill(uint8_t color) {
	memset(videoBuffer, color, sizeof(videoBuffer));
}

void Renderer::setPixel(uint16_t x, uint16_t y, uint8_t color) {
	videoBuffer[x + (y * Renderer::width)] = color;
}

uint8_t Renderer::getPixel(uint16_t x, uint16_t y) {
	return videoBuffer[x + (y * Renderer::width)];
}

void Renderer::togglePixel(uint16_t x, uint16_t y) {
	videoBuffer[x + (y * Renderer::width)] ^= 0xFF;
}