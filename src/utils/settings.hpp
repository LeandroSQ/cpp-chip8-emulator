#pragma once

#include <cstdint>

struct Settings {
	// Clock speed
	uint16_t cpuClockSpeed = 500;  // Hz
	uint16_t timerClockSpeed = 60; // Hz
	uint16_t frameRate = 60;	   // Hz

	// Graphics
	uint32_t backgroundColor = 0x9ec1a3ff;
	uint32_t foregroundColor = 0xcfe0c3ff;
	bool isFrameInterpolationEnabled = true;
    uint8_t frameInterpolationDecay = 0x80;
};