#include "app.hpp"
#include "../utils/util.hpp"
#include "../utils/logger.hpp"
#include <SDL2/SDL.h>

App::App() { }

App::~App() { }

int8_t App::init() {
	Log::info("[App] Starting...");

	// Initialize random number generator
	Random::init();

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
		Log::error("SDL could not initialize!");
		return 1;
	}

	// Initialize the emulator
	if (emulator.init() < 0) return 1;

	// Initialize the window
	if (window.init() < 0) return 1;

    return 0;
}

int8_t App::loadROM(const char* path) {
	// Read the binary file content
	uint8_t* buffer;
    size_t size;
	if (readFile(path, buffer, size) < 0) return -1;

	// Extract the filename from path, without extension
	currentRom = getFileName(path);

	// Load it into the emulator
	emulator.loadROM(buffer, size);

	// Free the buffer
	delete[] buffer;

	return 0;
}

void App::run() {
	Log::info("[App] Running...");
	isRunning = true;

	uint64_t lastFrameTime = SDL_GetPerformanceCounter();
	while (isRunning) {
		// Calculate delta time
		uint64_t frameStartTime = SDL_GetPerformanceCounter();
		deltaTime = (double(frameStartTime - lastFrameTime) / (double)SDL_GetPerformanceFrequency());
		lastFrameTime = frameStartTime;

		// Capture events
		window.pollEvents(emulator.input);

		// Stops execution if the window is trying to close
		if (window.isClosed) break;

		// Stops execution if the emulator is trying to halt
		cpuClock.update(deltaTime);
		if (emulator.isHalted) break;

		frameClock.update(deltaTime);
		cpuTimerClock.update(deltaTime);
		fpsClock.update(deltaTime);
	}

	Log::info("[App] Stopping...");
	isRunning = false;
}
