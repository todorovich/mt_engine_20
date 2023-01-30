export module Alarm;

import <chrono>;

import Engine;

export using namespace std::literals::chrono_literals;

namespace mt {
	namespace time { class AlarmManagerInterface; }
}

export namespace mt::time
{
	class Alarm
	{
	private:
		std::chrono::steady_clock::time_point _alarm_time;

		std::chrono::steady_clock::time_point _time_paused;

		mt::Task* _task;

		std::chrono::steady_clock::duration _reset_interval;

		bool _alarm_repeats;

		bool _has_triggered;

		bool _is_paused;

		static class DoNothing : public mt::Task {
			std::expected<void, mt::Error> operator()(){}
		} doNothing;

	public:

		friend AlarmManagerInterface;

		Alarm(
			std::chrono::steady_clock::time_point time_point, 
			mt::Task* task = &doNothing,
			bool alarm_repeats = false, 
			std::chrono::steady_clock::duration reset_interval = std::chrono::steady_clock::duration::min()
		) noexcept
			: _alarm_time(time_point)
			, _time_paused(std::chrono::steady_clock::time_point::min())
			, _task(task)
			, _reset_interval(reset_interval)
			, _alarm_repeats(alarm_repeats)
			, _is_paused(false)
		{}

		Alarm() noexcept
			: _alarm_time(std::chrono::steady_clock::time_point::min())
			, _time_paused(std::chrono::steady_clock::time_point::min())
			, _task(&doNothing)
			, _reset_interval(std::chrono::steady_clock::duration::min())
			, _alarm_repeats(false)
			, _is_paused(true) 
		{}
			
		~Alarm() noexcept = default;

		Alarm(const Alarm& other) noexcept = delete;
		
		Alarm(Alarm&& other) noexcept
			: _alarm_time(std::move(other._alarm_time))
			, _time_paused(std::move(other._time_paused))
			, _task(std::move(other._task))
			, _reset_interval(std::move(other._reset_interval))
			, _alarm_repeats(std::move(other._alarm_repeats))
			, _is_paused(std::move(other._is_paused))
		{};

		Alarm& operator=(const Alarm& other) noexcept = delete;

		Alarm& operator=(Alarm&& other)	noexcept
		{
			_alarm_time = std::move(other._alarm_time);
			_task = other._task;

			return *this;
		};

		bool operator<(const Alarm& other) const noexcept
		{
			return _alarm_time < other._alarm_time;
		}

		bool HasTriggered() const noexcept
		{ 
			return _has_triggered; 
		}

		bool doesAlarmRepeat() noexcept
		{
			return _alarm_repeats;
		}

		void tick(std::chrono::steady_clock::time_point current_tick_time = std::chrono::steady_clock::now());

		// used to reset an alarm that repeats
		void reset() noexcept;

		void pause(std::chrono::steady_clock::time_point time_paused = std::chrono::steady_clock::now()) noexcept;

		void resume(std::chrono::steady_clock::time_point time_continued = std::chrono::steady_clock::now()) noexcept;
	};

	struct AlarmCompare
	{
	public:
		bool operator()(Alarm* alarm_1, Alarm* alarm_2) const noexcept
		{
			return *alarm_2 < *alarm_1;
		}
	};
}

void mt::time::Alarm::tick(std::chrono::steady_clock::time_point current_tick_time)
{
	// Not triggered or paused
	if (_has_triggered == false && _is_paused == false)
	{
		if (current_tick_time >= _alarm_time)
		{
			_has_triggered = true;

			(*_task)();
		}
	}
}

void mt::time::Alarm::reset() noexcept
{
	_alarm_time += _reset_interval;
	_has_triggered = false;
}

void mt::time::Alarm::pause(std::chrono::steady_clock::time_point time_paused) noexcept
{
	_time_paused = time_paused;
	_is_paused = true;
}

void mt::time::Alarm::resume(std::chrono::steady_clock::time_point time_continued) noexcept
{
	// Offset time time by the amount of time spent paused;
	_alarm_time += time_continued - _time_paused;

	_time_paused = std::chrono::steady_clock::time_point::min();

	_is_paused = false;
}
