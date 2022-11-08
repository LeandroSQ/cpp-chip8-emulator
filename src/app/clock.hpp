#pragma once

#include <functional>

/**
 * @brief Abstraction class for a clock controller, calls a callback once per rate
 */
class Clock {
  private:
    uint16_t* timesPerSecond;
	double timer = 0.0;
	std::function<void()> callback;

    double getRate();

  public:
	uint16_t count = 0;

	Clock(uint16_t* timesPerSecond, std::function<void()> callback);
	Clock(std::function<void()> callback);

	~Clock();

	void update(double deltaTime);
};