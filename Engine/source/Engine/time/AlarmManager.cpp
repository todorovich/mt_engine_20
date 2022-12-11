module AlarmManager;

import Time;
import Alarm;
import ObjectPool;
import Engine;
import std.core;

using namespace std::literals::chrono_literals;

void mt::time::AlarmManager::addAlarm(TimePoint time_point, Task* callback, bool repeats, Duration repeat_interval)
{
	Alarm* alarm = _alarm_pool.allocate(_engine, time_point, callback, repeats, repeat_interval);

	_alarm_queue.push(alarm);
	_alarms_and_timers.insert(alarm);
}