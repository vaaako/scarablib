#include "scarablib/types/time/timer.hpp"


bool Timer::update(const float delta, const float interval) noexcept {
	// Avoid division by zero or negative intervals
	if(interval <= 0.0f) {
		return false;
	}

	this->accumulator += delta;
	if(this->accumulator >= interval) {
		this->accumulator -= interval; // Remove it to next frame
		return true;
	}
	return false;
}
