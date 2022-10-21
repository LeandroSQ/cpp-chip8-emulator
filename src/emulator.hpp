#pragma once
#include "audio.hpp"
#include "cpu.hpp"
#include "input.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "renderer.hpp"
#include "settings.hpp"

class Emulator {
  public:
	Settings settings;
	CPU cpu;
	Memory memory;
	Input input = Input(settings);
	Renderer renderer = Renderer(settings);
	Audio audio;

	bool isHalted = false;

	Emulator();

	~Emulator();

	int8_t init();

	void loadROM(const uint8_t* data);

	void cycle();

	void updateTimers();

	void reset();
};