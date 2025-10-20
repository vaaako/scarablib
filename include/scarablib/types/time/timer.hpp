#pragma once

// Manages a recurring interval
class Timer {
	public:
		Timer() noexcept = default;

		// Updates the timer with the elapsed time.
		// Returns true if the time reached or exceeded the interval.
		// - `delta`: Time elapsed since the last update (in seconds)
		// - `interval`: The desired time between events (in seconds)
		bool update(const float delta, const float interval) noexcept;

	private:
		float accumulator = 0.0f;
};
