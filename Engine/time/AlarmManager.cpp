module AlarmManager;

import <chrono>;

import Alarm;
import ObjectPool;
import Engine;

using namespace std::literals::chrono_literals;

void mt::time::AlarmManager::addAlarm(
	std::chrono::steady_clock::time_point time_point, 
	Task* callback, 
	bool repeats, 
	std::chrono::steady_clock::duration repeat_interval
)
{
	Alarm* alarm = _alarm_pool.allocate(_engine, time_point, callback, repeats, repeat_interval);

	_alarm_queue.push(alarm);
	_alarms_and_timers.insert(alarm);
}