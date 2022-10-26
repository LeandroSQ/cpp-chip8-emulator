#include "window.hpp"
#include "../utils/logger.hpp"

Window::Window() { }

Window::~Window() {
	// Free SDL resources
	SDL_DestroyRenderer(sdlRendererHandle);
	SDL_DestroyWindow(sdlWindowHandle);

	// Quit
	SDL_Quit();
}

int8_t Window::init() {
	Log::info("[Window] Initializing...");

	// Create the SDL window
	if (createWindow() < 0) return -1;

	// Create frame buffer
	if (createFrameBuffer() < 0) return -1;

	return 0;
}

int8_t Window::createWindow() {
	Log::info("[Window] Creating window...");

	// Create the window
	int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;

#ifdef ENABLE_HIGHDPI
	flags |= SDL_WINDOW_ALLOW_HIGHDPI;
#endif

	sdlWindowHandle = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, logicalViewport.width, logicalViewport.height, flags);
	if (sdlWindowHandle == nullptr) {
		Log::error("SDL could not create window!");
		return -1;
	}

	// Create the renderer
	sdlRendererHandle = SDL_CreateRenderer(sdlWindowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sdlRendererHandle == nullptr) {
		Log::error("SDL could not create Renderer!");
		return -1;
	}

	// Fetch the backend renderer context
	SDL_RendererInfo rendererInfo;
	SDL_GetRendererInfo(sdlRendererHandle, &rendererInfo);
	Log::info("[Window] SDL renderer backend: ", rendererInfo.name);
	Log::info("[Window] Viewport scaling: ", pixelScale, "x");

	return 0;
}

int8_t Window::createFrameBuffer() {
	Log::info("[Window] Allocating frame buffer...");

	// Create the texture
	frameBuffer = SDL_CreateTexture(
		sdlRendererHandle,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		logicalViewport.width,
		logicalViewport.height
	);

	if (frameBuffer == nullptr) {
		Log::error("SDL could not create Texture!");
		return -1;
	}

	return 0;
}

void Window::startFrame() {
	// Lock the texture and acquire the pixel data
	SDL_LockTexture(frameBuffer, nullptr, &pixels, &pitch);
}

void Window::endFrame() {
	// Unlocks the frame buffer
	SDL_UnlockTexture(frameBuffer);

	// Clear the back buffer
	SDL_RenderClear(sdlRendererHandle);
}

void Window::showFrame() {
	// Present the back buffer
	SDL_RenderPresent(sdlRendererHandle);
}

void Window::updateTitle(std::string title) {
	SDL_SetWindowTitle(sdlWindowHandle, title.c_str());
}

uint8_t* Window::getPixelsBuffer() {
	return (uint8_t*)pixels;
}