#pragma once
#include "logger.hpp"
#include "settings.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "audio.hpp"

class Emulator {
  public:
    Settings settings;
    CPU cpu;
    Memory memory;
    Input input = Input(settings);
    Renderer renderer = Renderer(settings);
    Audio audio;

    Emulator();

    ~Emulator();

    int8_t init();

    int8_t loadROM(const char* path);

    void cycle();

    void updateTimers(double deltaTime);

    void reset();
};