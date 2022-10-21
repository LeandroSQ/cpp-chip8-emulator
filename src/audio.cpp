#include "audio.hpp"
#include "logger.hpp"

constexpr int AMPLITUDE = 28000;
constexpr int SAMPLE_RATE = 44100;

Audio::Audio() {

}

Audio::~Audio() {
    SDL_CloseAudioDevice(device);
}

int8_t Audio::init() {
    Log::info("[Audio] Initializing...");

    SDL_AudioSpec desired;
    desired.freq = SAMPLE_RATE;
    desired.format = AUDIO_S16SYS;
    desired.channels = 1;
    desired.samples = 512;
    desired.callback = Audio::callback;
    desired.userdata = this;

    SDL_AudioSpec obtained;
    device = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desired, &obtained, 0);
    if (device <= 0) {
        Log::error("SDL could not open audio device!", SDL_GetError());
        return -1;
    }

    return 0;
}

void Audio::callback(void* context, uint8_t* stream, int bytes) {
    Audio* audio = (Audio*)context;

    int16_t* buffer = (int16_t*)stream;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS

    for (int i = 0; i < length; i++, audio->sampleIndex++) {
        double time = double(audio->sampleIndex) / double(SAMPLE_RATE);
        buffer[i] = (int16_t)(AMPLITUDE * sin(2.0f * M_PI * 440.0f * time));
    }
}

void Audio::reset() {
    sampleIndex = 0;
}

void Audio::play() {
    SDL_PauseAudioDevice(device, SDL_FALSE);
}

void Audio::stop() {
    SDL_PauseAudioDevice(device, SDL_TRUE);
}