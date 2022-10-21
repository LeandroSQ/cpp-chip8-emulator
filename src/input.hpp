#pragma once
#include <SDL2/SDL.h>
#include <cstdint>

class Input {
  private:
	bool keys[16] = { false };

  public:
	Input();

	~Input();

	void update(SDL_Event& event);

    void reset();

	bool isKeyDown(uint8_t key);

	bool isKeyUp(uint8_t key);

    uint8_t getFirstKeyDownIndex();
};