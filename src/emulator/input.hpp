#pragma once

#include "../utils/settings.hpp"
#include <SDL2/SDL.h>
#include <cstdint>

class Input {
  private:
	bool keys[16] = { false };
	Settings& settings;

  public:
	Input(Settings& settings);

	~Input();

	void update(SDL_Event& event);

	void reset();

    void setKey(uint8_t key, bool value);

	bool isKeyDown(uint8_t key);

	bool isKeyUp(uint8_t key);

	uint8_t getFirstKeyDownIndex();
};