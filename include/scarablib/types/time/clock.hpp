#pragma once

#include <chrono>
#include <functional>

// A flexible clock that provides delta time and manages callback-based alarms
class Clock {
	public:
		using HighResClock = std::chrono::high_resolution_clock;
		using Callback = std::function<void()>; // 

		Clock() noexcept;

		inline float get_delta() const noexcept {
			return this->delta;
		}

		// Call once per frame to update the clock and get the elapsed time in seconds.
		// Elapsed time is also stored, which can be get it with Clock::get_delta();
		float mark() noexcept;

		// Resets the clock's start time
		inline void reset() noexcept {
			this->last_time = HighResClock::now();
		}

	private:
		float delta;
		HighResClock::time_point last_time;

};
