#pragma warning( push )
#pragma warning( disable : 5050 )
module AlarmManager;

import std.core;
#pragma warning( pop )

import Time;
import Alarm;
import ObjectPool;
import Engine;

using namespace std::literals::chrono_literals;

void mt::time::AlarmManager::addAlarm(TimePoint time_point, Task* callback, bool repeats, Duration repeat_interval)
{
	Alarm* alarm = _alarm_pool.allocate(_engine, time_point, callback, repeats, repeat_interval);

	_alarm_queue.push(alarm);
	_alarms_and_timers.insert(alarm);
}