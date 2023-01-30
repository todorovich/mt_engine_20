export module AlarmManager;

export import <set>;
export import <queue>;
export import <chrono>;

export import Task;
export import Alarm;
using namespace std::literals::chrono_literals;

using std::chrono::steady_clock;

export namespace mt::time
{
	class AlarmManagerInterface
	{
	public:
		AlarmManagerInterface() noexcept = default;
		virtual ~AlarmManagerInterface() noexcept = default;
		AlarmManagerInterface(const AlarmManagerInterface& other) noexcept = default;
		AlarmManagerInterface(AlarmManagerInterface&& other) noexcept  = default;
		AlarmManagerInterface& operator=(const AlarmManagerInterface& other) noexcept = default;
		AlarmManagerInterface& operator=(AlarmManagerInterface&& other) noexcept = default;

		virtual void tick(steady_clock::time_point current_tick_time) noexcept = 0;

		virtual void pause(steady_clock::time_point time_paused = std::chrono::steady_clock::now()) noexcept = 0;

		virtual void resume(steady_clock::time_point time_resumed = std::chrono::steady_clock::now()) noexcept = 0;

		virtual void addAlarm(
			steady_clock::time_point time_point,
			Task* callback, 
			bool repeats = false, 
			steady_clock::duration repeat_interval = steady_clock::duration::min()
		) noexcept  = 0;
	};
}