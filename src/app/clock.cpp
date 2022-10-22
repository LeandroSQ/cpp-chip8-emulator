#include "clock.hpp"

Clock::Clock(double rate, std::function<void()> callback) : rate(rate), callback(callback) { }

Clock::~Clock() { }

void Clock::update(double deltaTime) {
	timer += deltaTime;

	// If the timer is greater than the rate, call the callback and reset the timer
	if (timer >= rate) {
		callback();

		timer -= rate;
		count++;
	}
}
