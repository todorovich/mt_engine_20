export module Alarm;

import Time;

export import std.core;

export using namespace std::literals::chrono_literals;

namespace mt::time { class AlarmManager; class TimeManager; }

export namespace mt::time
{
	class Alarm
	{
	private:
		TimePoint _alarm_time;

		TimePoint _time_paused;

		mt::Engine& _engine;

		Task* _callback;

		Duration _reset_interval;

		bool _alarm_repeats;

		bool _has_triggered;

		bool _is_paused;

		void tick(TimePoint current_tick_time);

	public:

		friend AlarmManager;

		Alarm(mt::Engine& engine, TimePoint time_point, Task callback = [](mt::Engine&) {}, bool alarm_repeats = false, Duration reset_interval = 0ns)
			: _engine(engine)
			, _alarm_time(time_point)
			, _time_paused(TimePoint(0ns))
			, _callback(callback)
			, _reset_interval(reset_interval)
			, _alarm_repeats(alarm_repeats)
			, _is_paused(false)
		{
			
		}

		Alarm(mt::Engine& engine)
			: _engine(engine)
			, _alarm_time(TimePoint::min())
			, _time_paused(TimePoint::min())
			, _callback([](mt::Engine&){})
			, _reset_interval(0ns)
			, _alarm_repeats(false)
			, _is_paused(true) 
		{

		}
			
		~Alarm() = default;

		Alarm(const Alarm& other) = delete;
		
		Alarm(Alarm&& other)
			: _engine(other._engine)
			, _alarm_time(std::move(other._alarm_time))
			, _time_paused(std::move(other._time_paused))
			, _callback(std::move(other._callback))
			, _reset_interval(std::move(other._reset_interval))
			, _alarm_repeats(std::move(other._alarm_repeats))
			, _is_paused(std::move(other._is_paused))
		{};

		Alarm& operator=(const Alarm& other) = delete;

		Alarm& operator=(Alarm&& other)	
		{
			_alarm_time = std::move(other._alarm_time);
			_callback = other._callback;

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

		void pause(TimePoint time_paused = Clock::now());

		void resume(TimePoint time_continued = Clock::now());
	};

	struct AlarmCompare
	{
	public:
		bool operator()(Alarm* alarm_1, Alarm* alarm_2) const
		{
			return *alarm_2 < *alarm_1;
		}
	};
}
