#include "app.hpp"
#include "../utils/util.hpp"
#include "../utils/logger.hpp"
#include "../utils/random.hpp"

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

    // Initialize the GUI
    if (gui.init() < 0) return 1;

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

    // Run the disassembler
    disassembledData = Disassembler::disassembleData(buffer, size);

	// Free the buffer
	delete[] buffer;

	return 0;
}

void App::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        // Check if the window is closed or the user pressed escape
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            Log::info("[App] Quit event received, exiting...");
            window.isClosed = true;
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            emulator.input.update(event);
        }

        gui.pollEvent(event);
    }
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

        // Calculate the frame time in milliseconds, store the Max
        const double frameTime = deltaTime * 1000.0;
        if (frameTime > lastMaxFrameTime) lastMaxFrameTime = frameTime;

		// Capture events
        handleEvents();

		// Stops execution if the window is trying to close
		if (window.isClosed) break;

		// Stops execution if the emulator is trying to halt
		cpuClock.update(deltaTime);
		if (emulator.isHalted) break;

        // Update clocks
		frameClock.update(deltaTime);
		cpuTimerClock.update(deltaTime);
		fpsClock.update(deltaTime);

        // Update GUI
        gui.update();
	}

	Log::info("[App] Stopping...");
	isRunning = false;
}
