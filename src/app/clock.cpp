#include "clock.hpp"

Clock::Clock(uint16_t* timesPerSecond, std::function<void()> callback) : timesPerSecond(timesPerSecond), callback(callback) { }

Clock::Clock(std::function<void()> callback) : callback(callback), timesPerSecond(nullptr) { }

Clock::~Clock() { }

double Clock::getRate() {
    if (timesPerSecond != nullptr) {
        return 1.0 / double(*timesPerSecond);
    } else {
        return 1.0;
    }
}

void Clock::update(double deltaTime) {
	timer += deltaTime;

	// If the timer is greater than the rate, call the callback and reset the timer
    double rate = getRate();
    if (timer >= rate) count++;
	while (timer >= rate) {
		callback();

		timer -= rate;
	}
}
