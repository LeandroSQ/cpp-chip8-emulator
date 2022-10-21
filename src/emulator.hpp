#pragma once
#include "logger.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "audio.hpp"

class Emulator {
  public:
    CPU cpu;
    Memory memory;
    Input input;
    Renderer renderer;
    Audio audio;

    Emulator();

    ~Emulator();

    int8_t init();

    int8_t loadROM(const char* path);

    void cycle();

    void updateTimers(double deltaTime);

    void reset();
};