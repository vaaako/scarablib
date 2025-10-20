#include "scarablib/types/time/clock.hpp"

Clock::Clock() noexcept : last_time(HighResClock::now()) {}

float Clock::mark() noexcept {
	const std::chrono::time_point now = HighResClock::now();
	const std::chrono::duration<float> delta = now - this->last_time;
	this->last_time = now;
	this->delta = delta.count();
	return this->delta;
}

