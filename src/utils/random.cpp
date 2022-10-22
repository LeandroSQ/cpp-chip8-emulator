#include "random.hpp"
#include "logger.hpp"
#include <iostream>

void Random::init() {
	Log::info("[Random] Initializing...");
	srand(time(NULL));
}

uint8_t Random::byte() {
	return rand() % 256;
}