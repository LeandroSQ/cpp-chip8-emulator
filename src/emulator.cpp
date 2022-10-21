#include "emulator.hpp"
#include <fstream>

Emulator::Emulator() {

}

Emulator::~Emulator() {

}

int8_t Emulator::init() {
    Log::info("[Emulator] Initializing...");

    if (audio.init() < 0) return -1;
    if (memory.init() < 0) return -1;

    return 0;
}

int8_t Emulator::loadROM(const char* path) {
    Log::info("[Emulator] Loading ROM '", path, "'...");

    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (file.good()) {
        // Get the file size
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Allocate memory for the ROM
        char* buffer = new char[size];

        // Read the ROM into the buffer
        if (file.read(buffer, size)) {
            // Copy the buffer into the memory
            for (long i = 0; i < size; i++) {
                memory.write(Memory::programStart + i, buffer[i]);
            }

            // Free the buffer
            delete[] buffer;

            return 0;
        } else {
            // Free the buffer
            delete[] buffer;

            Log::error("[Emulator] Failed to load ROM, couldn't read file!");
            return 1;
        }
    } else {
        Log::error("[Emulator] Failed to load ROM, couldn't open file!");
        return -1;
    }
}

void Emulator::cycle() {
    // Fetch the current opcode from memory
    uint16_t opcode = memory.read(cpu.programCounter) << 8 | memory.read(cpu.programCounter + 1);

    // Increment the program counter
    cpu.programCounter += sizeof(uint16_t);

    // Decode the opcode
    cpu.execute(opcode, memory, input, renderer);
}

void Emulator::updateTimers(double deltaTime) {
    cpu.updateTimers(deltaTime);

    // Check for sound timer
    if (cpu.soundTimer > 0) {
        audio.play();
    } else {
        audio.stop();
    }
}

void Emulator::reset() {
    cpu.reset();
    memory.reset();
    input.reset();
    renderer.fill(0x00);
    audio.reset();
}
