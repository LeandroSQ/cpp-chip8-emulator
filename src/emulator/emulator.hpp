#pragma once

#include "audio.hpp"
#include "cpu.hpp"
#include "input.hpp"
#include "../utils/logger.hpp"
#include "memory.hpp"
#include "renderer.hpp"
#include "../utils/settings.hpp"

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

	void loadROM(const uint8_t* data, size_t size);

	void cycle();

	void updateTimers();

	void reset();
};