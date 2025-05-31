#include "scarablib/types/timer.hpp"

Timer::Timer() noexcept : last_time(Clock::now()) {}


float Timer::calc_elapsed() noexcept {
	const auto now = Clock::now();
	const std::chrono::duration<float> delta = now - this->last_time;
	this->last_time = now;
	return delta.count(); // Return elapsed time
}

bool Timer::update_fps(const float target_fps) noexcept {
	// Avoid division by zero and infinity timer
	if(target_fps <= 0.0f) {
		return false;
	}

	accumulator += this->calc_elapsed(); // Add time passed
	if(accumulator >= (1.0f / target_fps)) {
		accumulator -= (1.0f / target_fps);
		return true;
	}

	return false;
}

bool Timer::update_interval(const float interval) noexcept {
	// Avoid division by zero and infinity timer
	if(interval <= 0.0f) {
		return false;
	}

	accumulator += this->calc_elapsed(); // Add time passed
	if(accumulator >= interval) {
		accumulator -= interval;
		return true;
	}

	return false;
}
