#pragma once

#include "../emulator/emulator.hpp"
#include "../gui/gui.hpp"
#include "../utils/settings.hpp"
#include "../emulator/disassembler.hpp"
#include "clock.hpp"
#include "window.hpp"
#include <string>
#include <vector>

class App {
  public:
	// Declare variables
	Settings settings;
	Window window;
	Emulator emulator;

    double deltaTime = 0.0;
	uint16_t fps = 0;
	uint16_t cycles = 0;
	double maxFrameTime = 0;

    // Misc
	bool isRunning = false;
	std::string currentRom;
    std::vector<Instruction> disassembledData;

  private:
	// GUI
	GUI gui = GUI(*this);

	// FPS Variables
	uint32_t lastMaxFrameTime = 0;
	Clock fpsClock = Clock(1.0, [&]() { // Once per second
		fps = frameClock.count;
		cycles = cpuClock.count;
        maxFrameTime = lastMaxFrameTime;
		lastMaxFrameTime = 0;

		cpuClock.count = 0;
		frameClock.count = 0;
		cpuTimerClock.count = 0;
	});

	// Frame clock
	Clock frameClock = Clock(1.0 / settings.frameRate, [&]() { // Once per frame
		window.startFrame();
		emulator.renderer.render(window);
		window.endFrame();
		gui.render();
		window.showFrame();
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

  private:
	void handleEvents();
};