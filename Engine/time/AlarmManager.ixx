export module AlarmManager;

import <set>;
import <queue>;
import <chrono>;

export import Alarm;
export import ObjectPool;

export using namespace std::literals::chrono_literals;

import Engine;

export namespace mt::time
{
	class AlarmManager
	{
	private:
		mt::ObjectPool<Alarm, 1024> _alarm_pool;

		std::priority_queue <Alarm*, std::vector <Alarm*>, AlarmCompare> _alarm_queue;

		std::set <Alarm*> _alarms_and_timers;

		mt::Engine& _engine;

	public:

		AlarmManager(mt::Engine& engine) noexcept
			: _engine(engine)
		{}

		~AlarmManager() noexcept = default;

		AlarmManager(const AlarmManager& other) noexcept = delete;

		AlarmManager(AlarmManager&& other) noexcept  = delete;

		AlarmManager& operator=(const AlarmManager& other) noexcept = delete;

		AlarmManager& operator=(AlarmManager&& other) noexcept = delete;

		void tick(std::chrono::steady_clock::time_point current_tick_time) noexcept
		{
			if (!_alarm_queue.empty())
			{
				Alarm* alarm = _alarm_queue.top();

				alarm->tick(current_tick_time);

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
					alarm->tick(current_tick_time);
				}
			}
		}

		void pause(std::chrono::steady_clock::time_point  time_paused = std::chrono::steady_clock::now()) noexcept
		{
			for (auto& alarm : _alarms_and_timers)
			{
				alarm->pause(time_paused);
			}
		}

		void resume(std::chrono::steady_clock::time_point  time_continued = std::chrono::steady_clock::now()) noexcept
		{
			for (auto& alarm : _alarms_and_timers)
			{
				alarm->resume(time_continued);
			}
		}

		void addAlarm(
			std::chrono::steady_clock::time_point time_point, 
			Task* callback, 
			bool repeats = false, 
			std::chrono::steady_clock::duration repeat_interval = std::chrono::steady_clock::duration::min()
		) noexcept ;
	};
}