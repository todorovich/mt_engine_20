export module Alarm;

import Time;

import std.core;
using namespace std::literals::chrono_literals;

namespace mt::time { class AlarmManager; }

export namespace mt::time
{
	class Alarm
	{
	private:
		TimePoint _alarm_time;

		TimePoint _time_paused;

		std::function<void()> _callback;

		Duration _reset_interval;

		bool _alarm_repeats;

		bool _has_triggered;

		bool _is_paused;

		void Tick(TimePoint current_tick_time, TimePoint previous_tick_time, Duration delta_time);

	public:

		friend AlarmManager;

		Alarm(TimePoint time_point, std::function<void()> callback = []() {}, bool alarm_repeats = false, Duration reset_interval = 0ns)
			: _alarm_time(time_point)
			, _time_paused(TimePoint(0ns))
			, _callback(callback)
			, _reset_interval(reset_interval)
			, _alarm_repeats(alarm_repeats)
			, _is_paused(false)
		{
			
		}

		Alarm() 
			: _alarm_time(TimePoint::min())
			, _time_paused(TimePoint::min())
			, _callback([](){})
			, _reset_interval(0ns)
			, _alarm_repeats(false)
			, _is_paused(true) 
		{

		}
			
		~Alarm() = default;

		Alarm(const Alarm& other) = delete;
		
		Alarm(Alarm&& other)
			: _alarm_time(std::move(other._alarm_time))
			, _callback(std::move(other._callback))
		{};

		Alarm& operator=(const Alarm& other) = delete;

		Alarm& operator=(Alarm&& other)	
		{
			_alarm_time = std::move(other._alarm_time);
			_callback = std::move(other._callback);

			return *this;
		};

		bool operator<(const Alarm& other) const
		{
			return _alarm_time < other._alarm_time;
		}

		bool HasTriggered() 
		{ 
			return _has_triggered; 
		}

		void Pause(TimePoint time_paused = Clock::now());

		void Continue(TimePoint time_continued = Clock::now());
	};
}

module :private;

void mt::time::Alarm::Tick(TimePoint current_tick_time, TimePoint previous_tick_time, Duration delta_time)
{
	// Not triggered or paused
	if (_has_triggered == false && _is_paused == false)
	{
		if (current_tick_time >= _alarm_time)
		{
			_has_triggered = true;

			_callback();
		}
	}
}

void mt::time::Alarm::Pause(TimePoint time_paused)
{
	_time_paused = time_paused;
	_is_paused = true;
}

void mt::time::Alarm::Continue(TimePoint time_continued)
{
	// Offset time time by the amount of time spent paused;
	_alarm_time += time_continued - _time_paused;

	_time_paused = TimePoint(0ns);

	_is_paused = false;
}