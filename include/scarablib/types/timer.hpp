#pragma once

#include <chrono>

// Multi-function timer.
// Do not use two timers at the same time
class Timer {
	using Clock = std::chrono::high_resolution_clock;

	public:
		// Multi-function timer.
		// Do not use two timers at the same time
		Timer() noexcept;

		// Returns how much time elapsed since last call
		inline float get_elapsed() noexcept {
			const auto now = Clock::now();
			const std::chrono::duration<float> delta = now - this->last_time;
			return delta.count();
		}

		// Resets timer and return elapsed time since last call
		float calc_elapsed() noexcept;

		// Resets timer to 0
		inline void reset_timer() noexcept {
			this->last_time = Clock::now();
		}

		// Call this every frame.
		// Timer is based on fps. Returns true if the timer should be updated.
		// - `target_fps`: e.g., animation fps target
		bool update_fps(const float target_fps) noexcept;

		// Call this every frame.
		// Returns true if timer reached the interval.
		// - `interval`: Time in seconds
		bool update_interval(const float interval) noexcept;

	private:
		Clock::time_point last_time;
		// Make two accumulators?
		float accumulator = 0.0f;
};
