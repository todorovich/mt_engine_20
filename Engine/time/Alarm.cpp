module Alarm;

import TimeManager;

import <chrono>;

using namespace std::literals;

void mt::time::Alarm::tick(std::chrono::steady_clock::time_point current_tick_time)
{
	// Not triggered or paused
	if (_has_triggered == false && _is_paused == false)
	{
		if (current_tick_time >= _alarm_time)
		{
			_has_triggered = true;

			_callback(_engine);
		}
	}
}

void mt::time::Alarm::pause(std::chrono::steady_clock::time_point time_paused)
{
	_time_paused = time_paused;
	_is_paused = true;
}

void mt::time::Alarm::resume(std::chrono::steady_clock::time_point time_continued)
{
	// Offset time time by the amount of time spent paused;
	_alarm_time += time_continued - _time_paused;

	_time_paused = std::chrono::steady_clock::time_point::min();

	_is_paused = false;
}
