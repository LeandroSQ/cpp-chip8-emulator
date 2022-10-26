#include "memory.hpp"
#include "../utils/logger.hpp"
#include "../utils/util.hpp"
#include <stdexcept>

Memory::Memory() { }

Memory::~Memory() { }

int8_t Memory::init() {
	loadFontset();

	return 0;
}

void Memory::loadFontset() {
	Log::info("[Memory] Loading fontset...");

	// Define the sizing
	constexpr uint8_t charCount = 16;
	constexpr uint8_t charSize = 5;

	// Define the fontset
	const uint8_t font[charCount][charSize] = {
		{ 0xF0, 0x90, 0x90, 0x90, 0xF0 }, /* 0 character */
		{ 0x20, 0x60, 0x20, 0x20, 0x70 }, /* 1 character */
		{ 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, /* 2 character */
		{ 0xF0, 0x10, 0xF0, 0x10, 0xF0 }, /* 3 character */
		{ 0x90, 0x90, 0xF0, 0x10, 0x10 }, /* 4 character */
		{ 0xF0, 0x80, 0xF0, 0x10, 0xF0 }, /* 5 character */
		{ 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, /* 6 character */
		{ 0xF0, 0x10, 0x20, 0x40, 0x40 }, /* 7 character */
		{ 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, /* 8 character */
		{ 0xF0, 0x90, 0xF0, 0x10, 0xF0 }, /* 9 character */
		{ 0xF0, 0x90, 0xF0, 0x90, 0x90 }, /* A character */
		{ 0xE0, 0x90, 0xE0, 0x90, 0xE0 }, /* B character */
		{ 0xF0, 0x80, 0x80, 0x80, 0xF0 }, /* C character */
		{ 0xE0, 0x90, 0x90, 0x90, 0xE0 }, /* D character */
		{ 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, /* E character */
		{ 0xF0, 0x80, 0xF0, 0x80, 0x80 }  /* F character */
	};

	// The fontset is loaded into memory starting at address 0x50, should be under 0x200
	// Therefore residing within the reserved memory
	for (uint8_t character = 0; character < charCount; character++) {
		for (uint8_t byte = 0; byte < charSize; byte++) {
			write(Memory::fontSetStart + (character * charSize) + byte, font[character][byte]);
		}
	}
}

uint8_t Memory::read(uint16_t address) {
	if (address >= sizeof(data)) {
		Log::error("[Memory] Address out of bounds: 0x", toHex(address, 4));
		// throw std::runtime_error("Memory::read: Address out of bounds");

		return 0x00;
	} else {
		return data[address];
	}
}

void Memory::write(uint16_t address, uint8_t value) {
	if (address >= sizeof(data)) {
		Log::error("[Memory] Address out of bounds: 0x", toHex(address, 4));
		// throw std::runtime_error("Memory::read: Address out of bounds");

		return;
	}

	data[address] = value;
}

void Memory::write(uint16_t address, uint8_t* value) {
	if (address >= sizeof(data)) {
		Log::error("[Memory] Address out of bounds: 0x", toHex(address, 4));
		// throw std::runtime_error("Memory::read: Address out of bounds");

		return;
	}

	for (unsigned long i = 0; i < sizeof(value); i++) {
		write(address + i, value[i]);
	}
}

void Memory::reset() {
	Log::debug("[Memory] Resetting memory...");

	memset(data, 0, sizeof(data));

	loadFontset();
}