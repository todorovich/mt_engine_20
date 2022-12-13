#pragma warning( push )
#pragma warning( disable : 5050 )
module Alarm;

import std.core;
#pragma warning( pop )

import TimeManager;

using namespace std::literals;

void mt::time::Alarm::tick(TimePoint current_tick_time)
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

void mt::time::Alarm::pause(TimePoint time_paused)
{
	_time_paused = time_paused;
	_is_paused = true;
}

void mt::time::Alarm::resume(TimePoint time_continued)
{
	// Offset time time by the amount of time spent paused;
	_alarm_time += time_continued - _time_paused;

	_time_paused = TimePoint(0ns);

	_is_paused = false;
}
