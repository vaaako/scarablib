#pragma once

#include "scarablib/typedef.hpp"
#include <chrono>
#include <functional>

class AlarmManager {
	public:
		using HighResClock = std::chrono::high_resolution_clock;
		using Callback = std::function<void()>;

		AlarmManager() noexcept;

		// Removes an alarm using its ID
		inline void cancel_alarm(const uint32 id) noexcept {
			std::erase_if(this->alarms, [id](const Alarm& alarm) {
				return alarm.id == id;
			});
		}

		// Sets an alarm that will trigger a callback after a set duration.
		// - `duration`: The time in seconds to wait before the callback is triggered.
		// - `repeat`: If true, the alarm will reset and fire again repeatedly
		// - `callback`: The function to call when the alarm fires.
		// Returns: A unique ID for the alarm, which can be used to cancel it
		uint32 add_alarm(const float duration, bool repeat, Callback callback) noexcept;

		// Call once per frame to update all alarms.
		// Takes delta time to calculate when to trigger alarms
		void update(const float delta) noexcept;
		
	private:
		struct Alarm {
			uint32 id;
			Callback callback;
			float interval;
			float accumulator;
			bool repeat;
		};

		std::vector<Alarm> alarms;
		uint32 curid = 0; // Current alarm id
};
