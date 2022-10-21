#pragma once

#include <SDL2/SDL.h>

class Audio {
  private:
	SDL_AudioDeviceID device;
	int sampleIndex = 0;

	static void callback(void* beeper, uint8_t* stream, int length);

  public:
	Audio();

	~Audio();

	int8_t init();

	void reset();

	void play();

	void stop();
};