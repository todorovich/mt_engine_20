export module StandardAlarmManager;

export import AlarmManager;

export import Engine;
export import ObjectPool;
export import Alarm;
export import Task;

using std::chrono::steady_clock;

export namespace mt::time 
{
	class StandardAlarmManager : public AlarmManagerInterface
	{
		mt::ObjectPool<Alarm, 1024> _alarm_pool;

		std::priority_queue <Alarm*, std::vector <Alarm*>, AlarmCompare> _alarm_queue;

		std::set <Alarm*> _alarms_and_timers;

		mt::Engine* _engine;
	public:
		StandardAlarmManager(mt::Engine* engine) noexcept
			: _engine(engine)
		{}

		virtual ~StandardAlarmManager() noexcept = default;
		StandardAlarmManager(const StandardAlarmManager& other) noexcept = delete;
		StandardAlarmManager(StandardAlarmManager&& other) noexcept  = delete;
		StandardAlarmManager& operator=(const StandardAlarmManager& other) noexcept = delete;
		StandardAlarmManager& operator=(StandardAlarmManager&& other) noexcept = delete;
		
		virtual void tick(steady_clock::time_point current_tick_time) noexcept override
		{
			if (!_alarm_queue.empty())
			{
				Alarm* alarm = _alarm_queue.top();

				alarm->tick(current_tick_time);

				while (alarm->HasTriggered())
				{
					_alarm_queue.pop();

					if (alarm->doesAlarmRepeat())
					{
						alarm->reset();
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

		virtual void pause(steady_clock::time_point time_paused = steady_clock::now()) noexcept override
		{
			for (auto& alarm: _alarms_and_timers)
			{
				alarm->pause(time_paused);
			}
		}

		virtual void resume(steady_clock::time_point time_resumed = steady_clock::now()) noexcept override
		{
			for (auto& alarm: _alarms_and_timers)
			{
				alarm->resume(time_resumed);
			}
		}

		void addAlarm(
			steady_clock::time_point time_point,
			Task* task,
			bool repeats = false,
			steady_clock::duration repeat_interval = std::chrono::steady_clock::duration::min()
		) noexcept
		{
			Alarm* alarm = _alarm_pool.allocate(time_point, task, repeats, repeat_interval);

			_alarm_queue.push(alarm);
			_alarms_and_timers.insert(alarm);
		}
	};
}