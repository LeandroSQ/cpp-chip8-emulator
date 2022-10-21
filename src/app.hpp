#pragma once

#include "clock.hpp"
#include "emulator.hpp"
#include "random.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "window.hpp"
#include <string>

class App {
  private:
	// Declare variables
	Settings settings;
	Window window;
	Emulator emulator;

	// Misc
	bool isRunning = false;
	std::string currentRom;

	// FPS Variables
	double deltaTime = 0.0;
	Clock fpsClock = Clock(1.0, [&]() { // Once per second
		window.updateTitle(
			"Chip8 Emulator " + currentRom + " | FPS: " + std::to_string(frameClock.count) +
			" | Cycles: " + std::to_string(cpuClock.count) + "/s"
		);
		cpuClock.count = 0;
		frameClock.count = 0;
	});

	// Frame clock
	Clock frameClock = Clock(1.0 / settings.frameRate, [&]() { // Once per frame
		window.startFrame();
		emulator.renderer.render(window);
		window.endFrame();
	});

	// CPU clock
	Clock cpuClock = Clock(1.0 / settings.cpuClockSpeed, [&]() { // Once per CPU cycle
		emulator.cycle();
	});

	// CPU timer clock
	Clock cpuTimerClock = Clock(1.0 / settings.timerClockSpeed, [&]() { // Once per CPU timer cycle
		emulator.updateTimers();
	});

  public:
	App();

	~App();

	int8_t init();

	int8_t loadROM(const char* path);

	void run();
};