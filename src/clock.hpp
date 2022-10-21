#pragma once
#include <functional>

/**
 * @brief Abstraction class for a clock controller, call a callback function once per rate
 */
class Clock {
  private:
	double rate = 0.0;
	double timer = 0.0;
	std::function<void()> callback;

  public:
	uint16_t count = 0;

	Clock(double rate, std::function<void()> callback);

	~Clock();

	void update(double deltaTime);
};