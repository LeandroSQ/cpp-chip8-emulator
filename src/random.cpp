#include "random.hpp"
#include <iostream>
#include "logger.hpp"

void Random::init() {
    Log::info("[Random] Initializing...");
	srand(time(NULL));
}

uint8_t Random::byte() {
	return rand() % 256;
}