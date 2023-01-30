export module Timer;

export import Alarm;

export import Task;

export namespace mt::time
{
	class Timer : public Alarm
	{		
	public:

		Timer(std::chrono::steady_clock::duration offset, mt::Task* callback) noexcept
			: Alarm(std::chrono::steady_clock::now() + offset, callback)
		{}

		~Timer() noexcept = default;

		Timer(const Timer& other) noexcept = delete;

		Timer(Timer&& other) noexcept
			: Alarm(std::move(other))
		{}

		Timer& operator=(const Timer& other) noexcept = delete;

		Timer& operator=(Timer&& other) noexcept
		{
			Alarm::operator=(std::move(other));

			return *this;
		};
	};
}