#include "window.hpp"
#include "logger.hpp"

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
	sdlWindowHandle = SDL_CreateWindow(
		"Chip8 Emulator",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		logicalViewport.width,
		logicalViewport.height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
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
		sdlRendererHandle, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, logicalViewport.width, logicalViewport.height
	);

	if (frameBuffer == nullptr) {
		Log::error("SDL could not create Texture!");
		return -1;
	}

	return 0;
}

void Window::pollEvents(Input& input) {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        Log::info("[Window] Window quit event received, exiting...");
        isClosed = true;
    } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        Log::debug("[Window] Key event received, updating emulator state...");
        input.update(event);
    }
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
	// Copy the texture into the back buffer
	SDL_RenderCopy(sdlRendererHandle, frameBuffer, nullptr, nullptr);
	// Present the back buffer
	SDL_RenderPresent(sdlRendererHandle);
}

void Window::updateTitle(std::string title) {
    SDL_SetWindowTitle(sdlWindowHandle, title.c_str());
}

uint8_t* Window::getPixelsBuffer() {
    return (uint8_t*)pixels;
}