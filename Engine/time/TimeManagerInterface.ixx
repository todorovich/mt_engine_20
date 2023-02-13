// Copyright 2022 Micho Todorovich, all rights reserved.
export module TimeManagerInterface;

export import <map>;
export import <string_view>;
export import <chrono>;
export import <expected>;

export import gsl;
export import TimeModel;

using namespace gsl;
using namespace mt::error;
using namespace mt::time::model;
using namespace mt::task;
using namespace std::literals;

// TODO: need a tick and a physics tick. 
export namespace mt::time
{
	class TimeManagerSetShouldUpdate;
	class TimeManagerSetShouldRender;
	class TimeManagerSetEndOfFrame;

	struct DefaultTimers {
		inline static const std::string_view RUN_TIME= "Run Time"sv;
		inline static const std::string_view WINDOWS_MESSAGE_TIME = "Windows Message Time"sv;
		inline static const std::string_view TICK_TIME = "Engine Tick Time"sv;
		inline static const std::string_view UPDATE_TIME = "Update Time"sv;
		inline static const std::string_view INPUT_TIME = "Input Time"sv;
		inline static const std::string_view RENDER_TIME = "Render Time"sv;
		inline static const std::string_view FRAME_TIME = "Frame Time"sv;
	};

	class NullTickFunction : public Task
	{
		virtual std::expected<void, Error> operator()() override { return {}; };
	};

	class TimeManagerInterface
	{
		std::chrono::steady_clock::duration	_update_interval_ns;
		std::chrono::steady_clock::duration	_render_interval_ns;
		std::chrono::steady_clock::duration	_frame_interval;
		std::chrono::steady_clock::duration	_command_list_interval;

		std::chrono::steady_clock::time_point _current_tick_time = std::chrono::steady_clock::time_point(0ns);
		std::chrono::steady_clock::time_point _previous_tick_time = std::chrono::steady_clock::time_point(0ns);

		std::chrono::steady_clock::duration _tick_delta_time_ns = 0ns;

		NullTickFunction _null_tick_function;

		not_null<Task*> _tick_function = &_null_tick_function;

		bool _is_paused;

		bool _should_update = false;
		bool _should_render = false;
		bool _end_of_frame = false;

	protected:
		[[nodiscard]] not_null<Task*> _getTickFunction() noexcept { return _tick_function; }
		[[nodiscard]] NullTickFunction& _getNullTickFunction() noexcept { return _null_tick_function; }

		void _setTickFunction(not_null<Task*> tick_function) noexcept
		{
			_tick_function = tick_function;
		}

		void _setShouldUpdate() noexcept
		{
			_should_update = true;
		}

		void _setShouldRender() noexcept
		{
			_should_render = true;
		}

		void _setEndOfFrame() noexcept
		{
			_end_of_frame = true;
		}

		void _setIsUpdatePaused(bool is_paused = true)
		{
			_is_paused = is_paused;
		}

		void _setCurrentTickTime(
			std::chrono::steady_clock::time_point current_tick_time = std::chrono::steady_clock::now()
		) noexcept
		{
			_current_tick_time = current_tick_time;
		};

		void _setPreviousTickTime(std::chrono::steady_clock::time_point prev_tick_time) noexcept
		{
			_previous_tick_time = prev_tick_time;
		};

		void _setTickDeltaTime(std::chrono::steady_clock::duration tick_delta_time_ns) noexcept
		{
			 _tick_delta_time_ns = tick_delta_time_ns;
		};

	public:
		friend mt::time::TimeManagerSetShouldUpdate;
		friend mt::time::TimeManagerSetShouldRender;
		friend mt::time::TimeManagerSetEndOfFrame;

		TimeManagerInterface()
			: _update_interval_ns(1'000'000'000ns / 60)
			, _render_interval_ns(1'000'000'000ns / 144)
			, _frame_interval (16666666ns)
			, _command_list_interval (0ns)
			, _is_paused(true)
		{

		}

		virtual ~TimeManagerInterface() noexcept = default;
		TimeManagerInterface(const TimeManagerInterface& other) noexcept = default;
		TimeManagerInterface(TimeManagerInterface&& other) noexcept = default;
		TimeManagerInterface& operator=(const TimeManagerInterface& other) noexcept = default;
		TimeManagerInterface& operator=(TimeManagerInterface&& other) noexcept = default;
	
		[[nodiscard]] std::chrono::steady_clock::time_point getCurrentTickTime() const noexcept
		{
			return _current_tick_time;
		};

		[[nodiscard]] std::chrono::steady_clock::time_point getPreviousTickTime() const noexcept
		{
			return _previous_tick_time;
		};

		[[nodiscard]] std::chrono::steady_clock::duration getTickDeltaTime() const noexcept
		{
			return _tick_delta_time_ns;
		};

		[[nodiscard]] std::chrono::steady_clock::duration getUpdateInterval() const noexcept
		{
			return _update_interval_ns;
		}

		// need to be able to detect if we are dropping frames and adjust this
		// accordingly
		[[nodiscard]] std::chrono::steady_clock::duration getRenderInterval() const noexcept
		{
			return _render_interval_ns;
		}

		[[nodiscard]] std::chrono::steady_clock::duration getFrameInterval() const noexcept
		{
			return _frame_interval;
		}

		std::chrono::steady_clock::time_point now() { return std::chrono::steady_clock::now(); }

		[[nodiscard]] bool getShouldUpdate() const { return _should_update; }
		[[nodiscard]] bool getShouldRender() const { return _should_render; }
		[[nodiscard]] bool getEndOfFrame() const { return _end_of_frame; }

		virtual void shutdown() noexcept = 0;

		void updateComplete() noexcept
		{
			_should_update = false;
		}

		void renderComplete() noexcept
		{
			_should_render = false;
		}

		void frameComplete() noexcept
		{
			_end_of_frame = false;
		}

		virtual void resume() noexcept = 0;		// Call to unpaused.
		virtual void pause() noexcept = 0;			// Call to pause.
		virtual void tick() noexcept
		{
			(*_tick_function)();
		}

		virtual StopWatch* findStopWatch(std::string_view name) = 0;

		[[nodiscard]] bool isUpdatePaused() const noexcept { return _is_paused; }
		//bool IsRenderPaused() const noexcept { return _is_render_paused; }
	};
}
