#pragma once
#include "window.hpp"

class Renderer {
  public:
	static constexpr uint8_t width = 64;
	static constexpr uint8_t height = 32;

	Renderer(Settings& settings);

	~Renderer();

	void render(Window& window);

	void fill(uint8_t color);

	void setPixel(uint16_t x, uint16_t y, uint8_t color);

	uint8_t getPixel(uint16_t x, uint16_t y);

    void togglePixel(uint16_t x, uint16_t y);

  private:
    Settings& settings;
	uint8_t videoBuffer[Renderer::width * Renderer::height] = { 0 };
	uint8_t lastFrameBuffer[Renderer::width * Renderer::height] = { 0 };

	void setPixel(const Window& window, uint8_t* pixels, uint16_t x, uint16_t y, uint8_t color);
};