#pragma once
#include <cstdint>

class Random {
  public:
    static void init();

    static uint8_t byte();
};