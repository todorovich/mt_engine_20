export module Alarm;

import <chrono>;

export using namespace std::literals::chrono_literals;

import Engine;

namespace mt {
	namespace time{ class AlarmManager; class TimeManager; }
}

export namespace mt::time
{
	class Alarm
	{
	private:
		std::chrono::steady_clock::time_point _alarm_time;

		std::chrono::steady_clock::time_point _time_paused;

		mt::Engine& _engine;

		Task* _callback;

		std::chrono::steady_clock::duration _reset_interval;

		bool _alarm_repeats;

		bool _has_triggered;

		bool _is_paused;

		void tick(std::chrono::steady_clock::time_point current_tick_time);

	public:

		friend AlarmManager;

		Alarm(
			mt::Engine& engine, 
			std::chrono::steady_clock::time_point time_point, 
			Task* callback = [](mt::Engine&) {}, 
			bool alarm_repeats = false, 
			std::chrono::steady_clock::duration reset_interval = std::chrono::steady_clock::duration::min()
		)
			: _engine(engine)
			, _alarm_time(time_point)
			, _time_paused(std::chrono::steady_clock::time_point::min())
			, _callback(callback)
			, _reset_interval(reset_interval)
			, _alarm_repeats(alarm_repeats)
			, _is_paused(false)
		{
			
		}

		Alarm(mt::Engine& engine)
			: _engine(engine)
			, _alarm_time(std::chrono::steady_clock::time_point::min())
			, _time_paused(std::chrono::steady_clock::time_point::min())
			, _callback([](mt::Engine&){})
			, _reset_interval(std::chrono::steady_clock::duration::min())
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

		void pause(std::chrono::steady_clock::time_point time_paused = std::chrono::steady_clock::now());

		void resume(std::chrono::steady_clock::time_point time_continued = std::chrono::steady_clock::now());
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
