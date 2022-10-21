#include "emulator.hpp"
#include "logger.hpp"
#include "random.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include <SDL2/SDL.h>

// Declare variables
Window window;
Emulator emulator;

// FPS Variables
double deltaTime = 0.0;
double fpsTimer = 0.0;

// Renderer frame counter
double frameTimer = 0.0;
double targetFrameRate = 1.0 / 60.0;
uint16_t frameCounter = 0;

// CPU cycle counter
double cpuTimer = 0.0;
double targetCPURate = 1.0 / 1000.0;
uint16_t cycleCounter = 0;

inline void handleCPUCycles() {
	// Update CPU clock timer
	cpuTimer += deltaTime;
	if (cpuTimer >= targetCPURate) {
		// Execute a cycle
		emulator.cycle();
		cycleCounter++;

		cpuTimer -= targetCPURate;
	}

    // Update emulator internal timers, they are updated at 60Hz
    emulator.updateTimers(deltaTime);
}

inline void handleRendererFrames() {
	// Update frame timer
	frameTimer += deltaTime;
	if (frameTimer >= targetFrameRate) {
		// Start rendering the frame
		window.startFrame();
		emulator.renderer.render(window);
		window.endFrame();
		frameCounter++;

		frameTimer -= targetFrameRate;
	}
}

inline void handleFPSTimer() {
	// Update FPS timer
	fpsTimer += deltaTime;
	if (fpsTimer >= 1.0) {
		fpsTimer -= 1.0;
		window.updateTitle(
			"Chip8 Emulator | FPS: " + std::to_string(frameCounter) + " | Cycles: " + std::to_string(cycleCounter) +
			"/s"
		);
		cycleCounter = 0;
		frameCounter = 0;
	}
}

inline int8_t handleArgs(int argc, char** argv) {
    Log::debug("[App] Parsing arguments...");
    Log::debug("[App] Argc: ", argc);
    for (int i = 0; i < argc; i++) {
        Log::debug("[App] Argv[", i, "]: ", argv[i]);
    }

    if (argc < 2) {
        Log::error("No ROM path provided");
        return -1;
    }

    if (emulator.loadROM(argv[1]) < 0) return -1;

    return 0;
}

int main(int argc, char** argv) {
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
    // Load custom ROM
	// if (emulator.loadROM("roms/invaders.bin") < 0) return 1;
    if (handleArgs(argc, argv) < 0) return 1;

	// Initialize the window
	if (window.init() < 0) return 1;

	uint64_t lastFrameTime = SDL_GetPerformanceCounter();
	while (true) {
		// Calculate delta time
		uint64_t frameStartTime = SDL_GetPerformanceCounter();
		deltaTime = (double(frameStartTime - lastFrameTime) / (double)SDL_GetPerformanceFrequency());
		lastFrameTime = frameStartTime;

		// Capture events
		window.pollEvents(emulator.input);

		// Stops execution if the window is trying to close
		if (window.isClosed) break;

        handleCPUCycles();
        handleRendererFrames();
		handleFPSTimer();
	}

	Log::info("[App] Stopping...");

	return 0;
}