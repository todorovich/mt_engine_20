export module Timer;

export import Time;
export import Alarm;

export namespace mt { class Engine; }

export namespace mt::time
{
	class Timer : public Alarm
	{		
	public:

		Timer(mt::Engine& engine, Duration offset, Task* callback)
			: Alarm(engine, Clock::now() + offset, callback)
		{}

		~Timer() = default;

		Timer(const Timer& other) = delete;

		Timer(Timer&& other)
			: Alarm(std::move(other))
		{}

		Timer& operator=(const Timer& other) = delete;

		Timer& operator=(Timer&& other)
		{
			Alarm::operator=(std::move(other));

			return *this;
		};
	};
}