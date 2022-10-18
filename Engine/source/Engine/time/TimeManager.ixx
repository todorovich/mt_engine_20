// Copyright 2022 Micho Todorovich, all rights reserved.
export module TimeManager;

export import Time;
export import Timer;
export import AlarmManager;
export import StopWatch;

export import std.core;

export using namespace std::literals::chrono_literals;

export namespace mt { class Engine; }

// TODO: need a tick and a physics tick. 
export namespace mt::time
{
	class TimeManager
	{
		void _DeleteAllChronometers();
		void _AddEngineAlarms();

		void _SetShouldUpdate();
		void _SetShouldRender();
		void _SetEndOfFrame();

		AlarmManager							_alarm_manager;
		std::map<std::string_view, StopWatch*>	_stop_watches;

		Duration	_tgt_update_interval_ns;
		Duration	_tgt_render_interval_ns;
		Duration	_frame_interval;
		Duration	_command_list_interval;

		TimePoint curr_tick_time = TimePoint(0ns);
		TimePoint prev_tick_time = TimePoint(0ns);

		Duration tick_delta_time_ns = 0ns;

		bool _is_paused;

		bool _should_update;
		bool _should_render;
		bool _end_of_frame;

	public:

		friend Engine;

		static struct DefaultTimers {
			static const std::string_view WINDOWS_MESSAGE_TIME;
			static const std::string_view FRAME_TIME;
			static const std::string_view TICK_TIME;
			static const std::string_view UPDATE_TIME;
			static const std::string_view INPUT_TIME;
			static const std::string_view RENDER_TIME;
		};

		TimeManager()
			: _tgt_update_interval_ns(16666666ns)
			, _tgt_render_interval_ns(1000000000ns/144)
			, _frame_interval (16666666ns)
			, _command_list_interval (0ns)
			, _is_paused(true)
			, _stop_watches{ 
				std::make_pair(DefaultTimers::FRAME_TIME, new StopWatch(*this, DefaultTimers::FRAME_TIME)),
				std::make_pair(DefaultTimers::WINDOWS_MESSAGE_TIME, new StopWatch(*this, DefaultTimers::WINDOWS_MESSAGE_TIME)),
				std::make_pair(DefaultTimers::TICK_TIME, new StopWatch(*this, DefaultTimers::TICK_TIME)),
				std::make_pair(DefaultTimers::UPDATE_TIME, new StopWatch(*this, DefaultTimers::UPDATE_TIME)),
				std::make_pair(DefaultTimers::INPUT_TIME, new StopWatch(*this, DefaultTimers::INPUT_TIME)),
				std::make_pair(DefaultTimers::RENDER_TIME, new StopWatch(*this, DefaultTimers::RENDER_TIME)),
			}
		{
			_AddEngineAlarms();

			curr_tick_time = Clock::now();
			prev_tick_time = TimePoint(0ns);

			tick_delta_time_ns = 0ns;

			_is_paused = false;
		}

		~TimeManager()
		{
			_DeleteAllChronometers();
		}
	
		TimePoint GetCurrentTickTime() const { return curr_tick_time; };

		TimePoint GetPreviousTickTime() const { return prev_tick_time; };

		Duration GetTickDeltaTime() const { return tick_delta_time_ns; };

		Duration GetTargetUpdateInterval() const { return _tgt_update_interval_ns; }

		Duration GetTargetRenderInterval() const { return _tgt_render_interval_ns; }

		bool GetShouldUpdate() const { return _should_update; }
		bool GetShouldRender() const { return _should_render; }
		bool GetEndOfFrame() const { return _end_of_frame; }

		void Continue();		// Call to unpaused.
		void Pause();			// Call to pause.
		void Tick();			// Call every frame.
	
		void UpdateComplete();
		void RenderComplete();
		void FrameComplete();

		StopWatch& FindTimer(std::string_view name) 
		{ 
			auto find = _stop_watches.find(name);
			if (find == _stop_watches.end())
			{
				throw;
			}
			else
			{
				return *find->second;	
			}
		}

		bool IsUpdatePaused() const { return _is_paused; }
		//bool IsRenderPaused() const { return _is_render_paused; }
	};
}
