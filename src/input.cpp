#include "input.hpp"
#include "logger.hpp"

Input::Input(Settings& settings) : settings(settings) { }

Input::~Input() { }

void Input::update(SDL_Event& event) {
	bool isKeyDown = event.type == SDL_KEYDOWN;

	if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
		Log::info("[Window] Toggling frame interpolation ", settings.isFrameInterpolationEnabled ? "off" : "on");
		settings.isFrameInterpolationEnabled = !settings.isFrameInterpolationEnabled;
	}

	switch (event.key.keysym.sym) {
		case SDLK_1:
			if (keys[0x1] != isKeyDown) {
				Log::debug("[Input] Key '1' is ", isKeyDown ? "down" : "up");
				keys[0x1] = isKeyDown;
			}
			break;
		case SDLK_2:
			if (keys[0x2] != isKeyDown) {
				Log::debug("[Input] Key '2' is ", isKeyDown ? "down" : "up");
				keys[0x2] = isKeyDown;
			}
			break;
		case SDLK_3:
			if (keys[0x3] != isKeyDown) {
				Log::debug("[Input] Key '3' is ", isKeyDown ? "down" : "up");
				keys[0x3] = isKeyDown;
			}
			break;
		case SDLK_4:
			if (keys[0xC] != isKeyDown) {
				Log::debug("[Input] Key '4' is ", isKeyDown ? "down" : "up");
				keys[0xC] = isKeyDown;
			}
			break;
		case SDLK_q:
			if (keys[0x4] != isKeyDown) {
				Log::debug("[Input] Key 'q' is ", isKeyDown ? "down" : "up");
				keys[0x4] = isKeyDown;
			}
			break;
		case SDLK_w:
			if (keys[0x5] != isKeyDown) {
				Log::debug("[Input] Key 'w' is ", isKeyDown ? "down" : "up");
				keys[0x5] = isKeyDown;
			}
			break;
		case SDLK_e:
			if (keys[0x6] != isKeyDown) {
				Log::debug("[Input] Key 'e' is ", isKeyDown ? "down" : "up");
				keys[0x6] = isKeyDown;
			}
			break;
		case SDLK_r:
			if (keys[0xD] != isKeyDown) {
				Log::debug("[Input] Key 'r' is ", isKeyDown ? "down" : "up");
				keys[0xD] = isKeyDown;
			}
			break;
		case SDLK_a:
			if (keys[0x7] != isKeyDown) {
				Log::debug("[Input] Key 'a' is ", isKeyDown ? "down" : "up");
				keys[0x7] = isKeyDown;
			}
			break;
		case SDLK_s:
			if (keys[0x8] != isKeyDown) {
				Log::debug("[Input] Key 's' is ", isKeyDown ? "down" : "up");
				keys[0x8] = isKeyDown;
			}
			break;
		case SDLK_d:
			if (keys[0x9] != isKeyDown) {
				Log::debug("[Input] Key 'd' is ", isKeyDown ? "down" : "up");
				keys[0x9] = isKeyDown;
			}
			break;
		case SDLK_f:
			if (keys[0xE] != isKeyDown) {
				Log::debug("[Input] Key 'f' is ", isKeyDown ? "down" : "up");
				keys[0xE] = isKeyDown;
			}
			break;
		case SDLK_z:
			if (keys[0xA] != isKeyDown) {
				Log::debug("[Input] Key 'z' is ", isKeyDown ? "down" : "up");
				keys[0xA] = isKeyDown;
			}
			break;
		case SDLK_x:
			if (keys[0x0] != isKeyDown) {
				Log::debug("[Input] Key 'x' is ", isKeyDown ? "down" : "up");
				keys[0x0] = isKeyDown;
			}
			break;
		case SDLK_c:
			if (keys[0xB] != isKeyDown) {
				Log::debug("[Input] Key 'c' is ", isKeyDown ? "down" : "up");
				keys[0xB] = isKeyDown;
			}
			break;
		case SDLK_v:
			if (keys[0xF] != isKeyDown) {
				Log::debug("[Input] Key 'v' is ", isKeyDown ? "down" : "up");
				keys[0xF] = isKeyDown;
			}
			break;
	}
}

void Input::reset() {
	Log::info("[Input] Resetting input state...");

	memset(keys, 0, sizeof(keys));
}

bool Input::isKeyDown(uint8_t key) {
	return keys[key];
}

bool Input::isKeyUp(uint8_t key) {
	return !keys[key];
}

uint8_t Input::getFirstKeyDownIndex() {
	for (unsigned long i = 0; i < sizeof(keys); i++) {
		if (keys[i]) {
			Log::debug("[Input] First key down index is ", i);

			return i;
		}
	}

	return 0xFF;
}