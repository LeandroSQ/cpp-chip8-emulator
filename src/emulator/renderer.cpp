#include "renderer.hpp"
#include "../utils/logger.hpp"
#include "../utils/util.hpp"
#include <SDL2/SDL.h>

Renderer::Renderer(Settings& settings) : settings(settings) {}

Renderer::~Renderer() { }

void Renderer::render(Window& window) {
	uint8_t* pixels = window.getPixelsBuffer();
    uint8_t* foreground = extractColorChannels(settings.foregroundColor);
    uint8_t* background = extractColorChannels(settings.backgroundColor);

    // Copy the video buffer to the frame buffer texture
    for (uint8_t y = 0; y < window.viewport.height; y++) {
		for (uint8_t x = 0; x < window.viewport.width; x++) {
			uint16_t index = (y * window.viewport.width) + x;
            uint8_t& rawPixel = videoBuffer[index];
			uint8_t& bufferedPixel = lastFrameBuffer[index];

            // If interpolation is enabled copy to the frame buffer
            if (!settings.isFrameInterpolationEnabled) bufferedPixel = rawPixel;
            else if (rawPixel == 0xFF) bufferedPixel = 0xFF;

            // Extract the color channels and apply it to the screen
            uint8_t* channels = bufferedPixel != 0x00 ? foreground : background;
            setPixel(window, pixels, x, y, channels[0], channels[1], channels[2], bufferedPixel);

            // Fade the pixel if it's not 0xFF
            if (settings.isFrameInterpolationEnabled && rawPixel != 0xFF) {
                if (bufferedPixel > settings.frameInterpolationDecay)
                    bufferedPixel -= settings.frameInterpolationDecay;
                else
                    bufferedPixel = 0x00;
            }
        }
    }

    delete[] foreground;
    delete[] background;
}

void Renderer::setPixel(const Window& window, uint8_t* pixels, uint16_t x, uint16_t y, uint8_t colorR, uint8_t colorG, uint8_t colorB, uint8_t colorA) {
	for (int scaleX = 0; scaleX < window.pixelScale; scaleX++) {
		for (int scaleY = 0; scaleY < window.pixelScale; scaleY++) {
			int realX = x * window.pixelScale + scaleX;
			int realY = y * window.pixelScale + scaleY;
			int totalOffset = (realX + realY * window.logicalViewport.width) * (sizeof(uint8_t) * 4);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			// ARGB
			pixels[totalOffset + 0] = colorA;
			pixels[totalOffset + 1] = colorR;
			pixels[totalOffset + 2] = colorG;
			pixels[totalOffset + 3] = colorB;
#else
			// BGRA
			pixels[totalOffset + 0] = colorB;
			pixels[totalOffset + 1] = colorG;
			pixels[totalOffset + 2] = colorR;
			pixels[totalOffset + 3] = colorA;
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