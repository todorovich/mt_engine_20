export module Timer;

export import Alarm;

import Engine;

export namespace mt::time
{
	class Timer : public Alarm
	{		
	public:

		Timer(mt::Engine& engine, std::chrono::steady_clock::duration offset, Task* callback)
			: Alarm(engine, std::chrono::steady_clock::now() + offset, callback)
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