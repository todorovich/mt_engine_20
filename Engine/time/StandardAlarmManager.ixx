export module StandardAlarmManager;

import std;

export import AlarmManager;

export import gsl;
export import Error;
export import ObjectPool;
export import TimeModel;

import MakeUnique;

using namespace gsl;

using std::chrono::steady_clock;
using namespace mt::error;
using namespace mt::memory;
using namespace mt::time::model;

export namespace mt::time
{
	class StandardAlarmManager : public AlarmManagerInterface
	{
		mt::memory::ObjectPool<Alarm, 1024> _alarm_pool;

		std::priority_queue < Alarm*, std::vector<Alarm*>, AlarmCompare > _alarm_queue;

		std::map<Alarm*, ObjectPool<Alarm, 1024>::unique_ptr_t> _alarms_and_timers;

	public:
		StandardAlarmManager(std::error_condition& error) noexcept
			: _alarm_pool(error)
		{}

		virtual ~StandardAlarmManager() noexcept = default;
		StandardAlarmManager(const StandardAlarmManager& other) noexcept = delete;
		StandardAlarmManager(StandardAlarmManager&& other) noexcept  = delete;
		StandardAlarmManager& operator=(const StandardAlarmManager& other) noexcept = delete;
		StandardAlarmManager& operator=(StandardAlarmManager&& other) noexcept = delete;
		
		void tick(steady_clock::time_point current_tick_time) noexcept override
		{
			if (!_alarm_queue.empty())
			{
				Alarm* alarm = _alarm_queue.top();

				alarm->tick(current_tick_time);

				while (alarm->HasTriggered())
				{
					if (alarm->doesAlarmRepeat())
					{
						alarm->reset();
						_alarm_queue.push(alarm);
					}
					else
					{
						// This causes the smart pointer to release
						_alarms_and_timers.erase(alarm);
					}

					// this will trigger a release, the unique_ptr is popped from the list and  ownership is not transferred.
					_alarm_queue.pop();
					alarm = _alarm_queue.top();
					alarm->tick(current_tick_time);
				}
			}
		}

		void pause(steady_clock::time_point time_paused = steady_clock::now()) noexcept override
		{
			for (auto& alarm: _alarms_and_timers)
			{
				alarm.second->pause(time_paused);
			}
		}

		void resume(steady_clock::time_point time_resumed = steady_clock::now()) noexcept override
		{
			for (auto& alarm: _alarms_and_timers)
			{
				alarm.second->resume(time_resumed);
			}
		}

		void addAlarm(
			steady_clock::time_point time_point,
			not_null<mt::task::Task *> task,
			bool repeats = false,
			steady_clock::duration repeat_interval = std::chrono::steady_clock::duration::min()
		) noexcept
		{
			auto alarm = _alarm_pool.allocate(time_point, task, repeats, repeat_interval);
			_alarm_queue.push(alarm.get());
			_alarms_and_timers.insert({alarm.get(), std::move(alarm)});
		}
	};
}