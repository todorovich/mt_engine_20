export module AlarmManager;

export import Time;
export import Alarm;
export import ObjectPool;
export import std.core;

export using namespace std::literals::chrono_literals;

export namespace mt::time
{
	struct AlarmCompare
	{
	public:
		bool operator()(Alarm* alarm_1, Alarm* alarm_2) const
		{
			return *alarm_2 < *alarm_1;
		}
	};

	export class AlarmManager
	{
	private:
		mt::ObjectPool<Alarm, 1024>				_alarm_pool;

		std::priority_queue <Alarm*, std::vector <Alarm*>, AlarmCompare>	_alarm_queue;

		std::set <Alarm*>													_alarms_and_timers;

	public:

		AlarmManager() = default;

		~AlarmManager() = default;

		AlarmManager(const AlarmManager& other) = delete;

		AlarmManager(AlarmManager&& other) = delete;

		AlarmManager& operator=(const AlarmManager& other) = delete;

		AlarmManager& operator=(AlarmManager&& other) = delete;

		void tick(TimePoint current_tick_time, TimePoint previous_tick_time, Duration delta_time)
		{
			if (!_alarm_queue.empty())
			{
				Alarm* alarm = _alarm_queue.top();

				alarm->tick(current_tick_time, previous_tick_time, delta_time);

				while (alarm->HasTriggered())
				{
					_alarm_queue.pop();

					if (alarm->_alarm_repeats == true)
					{
						alarm->_alarm_time += alarm->_reset_interval;
						alarm->_has_triggered = false;
						_alarm_queue.push(alarm);
					}
					else
					{
						_alarm_pool.releaseMemory(alarm);
						_alarms_and_timers.erase(alarm);
					}

					alarm = _alarm_queue.top();
					alarm->tick(current_tick_time, previous_tick_time, delta_time);
				}
			}
		}

		void pause(TimePoint time_paused = Clock::now())
		{
			for (auto& alarm : _alarms_and_timers)
			{
				alarm->pause(time_paused);
			}
		}


		void resume(TimePoint time_continued = Clock::now())
		{
			for (auto& alarm : _alarms_and_timers)
			{
				alarm->resume(time_continued);
			}
		}

		void addAlarm(TimePoint time_point, std::function<void()> function, bool repeats = false, Duration repeat_interval = 0ns)
		{
			Alarm* alarm = new (_alarm_pool.getMemory()) Alarm(time_point, function, repeats, repeat_interval);

			_alarm_queue.push(alarm);
			_alarms_and_timers.insert(alarm);
		}
	};
}