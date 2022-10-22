#pragma once

#include <cstdint>

struct Settings {
	// Clock speed
	uint16_t cpuClockSpeed = 500;  // Hz
	uint16_t timerClockSpeed = 60; // Hz
	uint16_t frameRate = 60;	   // Hz

	// Graphics
	uint32_t backgroundColor = 0x000000;
	uint32_t foregroundColor = 0xFFFFFF;
	bool isFrameInterpolationEnabled = false;
};