#include "scarablib/types/time/alarmmanager.hpp"

AlarmManager::AlarmManager() noexcept {}


uint32 AlarmManager::add_alarm(const float duration, bool repeat, Callback callback) noexcept {
	if(duration <= 0.0f) {
		return 0; // Returning a int -1 would do the same here
	}

	uint32 id = this->curid++;
	alarms.push_back({
		.id = id,
		.callback = std::move(callback),
		.interval = duration,
		.accumulator = 0.0f,
		.repeat = repeat
	});
	return id;
}

void AlarmManager::update(const float delta) noexcept {
	std::vector<uint32> alarms_to_remove;
	for(Alarm& alarm : alarms) {
		alarm.accumulator += delta;

		// Use a while here to handle large delta times
		// and an alarm that might need to fire multiple times
		while(alarm.accumulator >= alarm.interval) {
			alarm.callback(); // Trigger the callback
			if(alarm.repeat) {
				alarm.accumulator -= alarm.interval; // Reset for the next interval
				continue;
			}

			// Remove oneshot alarm
			alarms_to_remove.emplace_back(alarm.id);
			break;
		}
	}

	for(uint32 id : alarms_to_remove) {
		this->cancel_alarm(id);
	}
}

