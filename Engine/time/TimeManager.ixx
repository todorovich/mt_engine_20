// Copyright 2022 Micho Todorovich, all rights reserved.
export module TimeManager;

export import <map>;
export import <string_view>;
export import <chrono>;

export import Timer;
export import AlarmManager;
export import StopWatch;

export using namespace std::literals;

import Engine;

// TODO: need a tick and a physics tick. 
export namespace mt::time
{
	class TimeManager
	{
		void _addEngineAlarms();

		void _setShouldUpdate();
		void _setShouldRender();
		void _setEndOfFrame();

		AlarmManager											_alarm_manager;
		std::map<std::string_view, std::unique_ptr<StopWatch>>	_stop_watches;

		std::chrono::steady_clock::duration	_tgt_update_interval_ns;
		std::chrono::steady_clock::duration	_tgt_render_interval_ns;
		std::chrono::steady_clock::duration	_frame_interval;
		std::chrono::steady_clock::duration	_command_list_interval;

		std::chrono::steady_clock::time_point curr_tick_time = std::chrono::steady_clock::time_point(0ns);
		std::chrono::steady_clock::time_point prev_tick_time = std::chrono::steady_clock::time_point(0ns);

		std::chrono::steady_clock::duration tick_delta_time_ns = 0ns;

		bool _is_paused;

		bool _should_update;
		bool _should_render;
		bool _end_of_frame;

		mt::Engine& _engine;

	public:

		friend Engine;

		struct DefaultTimers {
			static const std::string_view RUN_TIME; 
			static const std::string_view WINDOWS_MESSAGE_TIME;
			static const std::string_view TICK_TIME;
			static const std::string_view UPDATE_TIME;
			static const std::string_view INPUT_TIME;
			static const std::string_view RENDER_TIME;
			static const std::string_view FRAME_TIME;
		};

		TimeManager(mt::Engine& engine)
			: _engine(engine)
			, _alarm_manager(engine)
			, _tgt_update_interval_ns(1'000'000'000ns/60)
			, _tgt_render_interval_ns(1'000'000'000ns/144)
			, _frame_interval (16666666ns)
			, _command_list_interval (0ns)
			, _is_paused(true)
			, _stop_watches()
		{
			_stop_watches.emplace(std::make_pair(DefaultTimers::RUN_TIME,				std::make_unique<StopWatch>(*this, DefaultTimers::RUN_TIME)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::WINDOWS_MESSAGE_TIME,	std::make_unique<StopWatch>(*this, DefaultTimers::WINDOWS_MESSAGE_TIME)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::TICK_TIME,				std::make_unique<StopWatch>(*this, DefaultTimers::TICK_TIME)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::UPDATE_TIME,			std::make_unique<StopWatch>(*this, DefaultTimers::UPDATE_TIME)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::INPUT_TIME,				std::make_unique<StopWatch>(*this, DefaultTimers::INPUT_TIME)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::RENDER_TIME,			std::make_unique<StopWatch>(*this, DefaultTimers::RENDER_TIME)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::FRAME_TIME,				std::make_unique<StopWatch>(*this, DefaultTimers::FRAME_TIME)));

			_addEngineAlarms();

			curr_tick_time = std::chrono::steady_clock::now();
			prev_tick_time = std::chrono::steady_clock::time_point::min();

			tick_delta_time_ns = std::chrono::steady_clock::duration::min();

			_is_paused = false;
		}

		~TimeManager() = default;
	
		std::chrono::steady_clock::time_point getCurrentTickTime() const { return curr_tick_time; };

		std::chrono::steady_clock::time_point getPreviousTickTime() const { return prev_tick_time; };

		std::chrono::steady_clock::duration getTickDeltaTime() const { return tick_delta_time_ns; };

		std::chrono::steady_clock::duration getTargetUpdateInterval() const { return _tgt_update_interval_ns; }

		// need to be able to detect if we are dropping frames and adjust this
		// accordingly
		std::chrono::steady_clock::duration getTargetRenderInterval() const { return _tgt_render_interval_ns; }

		bool getShouldUpdate() const { return _should_update; }
		bool getShouldRender() const { return _should_render; }
		bool getEndOfFrame() const { return _end_of_frame; }

		void resume();		// Call to unpaused.
		void pause();			// Call to pause.
		void tick();			// Call every frame.
	
		void updateComplete();
		void renderComplete();
		void frameComplete();

		StopWatch* findStopWatch(std::string_view name) 
		{ 
			auto find = _stop_watches.find(name);
			if (find == _stop_watches.end())
			{
				throw;
			}
			else
			{
				return find->second.get();	
			}
		}

		bool IsUpdatePaused() const { return _is_paused; }
		//bool IsRenderPaused() const { return _is_render_paused; }

		Engine& getEngine()
		{
			return _engine;
		}
	};
}
