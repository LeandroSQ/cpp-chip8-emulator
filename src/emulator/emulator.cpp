#include "emulator.hpp"

Emulator::Emulator() { }

Emulator::~Emulator() { }

int8_t Emulator::init() {
	Log::info("[Emulator] Initializing...");

	if (audio.init() < 0) return -1;
	if (memory.init() < 0) return -1;

	return 0;
}

void Emulator::loadROM(const uint8_t* data, size_t size) {
	Log::info("[Emulator] Loading ROM...");

	// Reset the emulator
	reset();

	// Load the ROM into memory
	for (size_t i = 0; i < size; i++) {
		memory.write(Memory::programStart + i, data[i]);
	}
}

void Emulator::cycle() {
	// Check if the emulator is halted
	if (cpu.programCounter >= memory.size) {
		Log::error("[Emulator] Program counter out of bounds!");
		isHalted = true;
		return;
	}

	// Fetch the current opcode from memory
	uint16_t opcode = memory.read(cpu.programCounter) << 8 | memory.read(cpu.programCounter + 1);

    Log::debug("[Emulator] Opcode: 0x", toHex(opcode, 4));

	// Increment the program counter
	cpu.programCounter += sizeof(uint16_t);

	// Decode the opcode
	cpu.execute(opcode, memory, input, renderer);
}

void Emulator::updateTimers() {
	cpu.updateTimers();

	// Check for sound timer
	if (cpu.soundTimer > 0) {
		audio.play();
	} else {
		audio.stop();
	}
}

void Emulator::reset() {
	isHalted = false;
	cpu.reset();
	memory.reset();
	input.reset();
	renderer.fill(0x00);
	audio.reset();
}
