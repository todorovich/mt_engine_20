// Copyright 2022 Micho Todorovich, all rights reserved.
export module TimeManager;

export import Time;
export import Timer;
export import AlarmManager;
export import Chronometer;

export import std.core;

export using namespace std::literals::chrono_literals;

export namespace mt::time
{
	class TimeManager
	{
		static const int _number_of_samples_to_use = 100;
	
	public:
		TimeManager()
			: _tgt_update_interval_ns(8333333ns)
			, _tgt_render_interval_ns(16666666ns)
			, _frame_interval (16666666ns)
			, _command_list_interval (0ns)
			, _is_paused(true)
		{}

		~TimeManager()
		{
			_DeleteAllChronometers();
		}
	
		TimePoint GetCurrentTickTime() const { return curr_tick_time; };

		TimePoint GetPreviousTickTime() const { return prev_tick_time; };

		Duration GetTickDeltaTime() const { return tick_delta_time_ns; };

		Duration DurationSinceLaunch() const { return _chronometers.find(std::string("Total Up-Time"))->second->GetDurationSinceStarted(); };

		Duration GetTargetUpdateInterval() const { return _tgt_update_interval_ns; }

		Duration GetTargetRenderInterval() const { return _tgt_render_interval_ns; }

		bool GetShouldUpdate() const { return _should_update; }
		bool GetShouldRender() const { return _should_render; }
		bool GetEndOfFrame() const { return _end_of_frame; }

		void Initialize();		// Call before message loop.
		void Continue();		// Call to unpaused.
		void Pause();			// Call to pause.
		void Tick();			// Call every frame.
	
		void UpdateComplete();
		void RenderComplete();
		void FrameComplete();

		Chronometer& FindTimer(std::string name) 
		{ 
			auto find = _chronometers.find(name);
			if (find == _chronometers.end())
			{
				throw;
			}
			else
			{
				return *find->second;	
			}
		}

		Chronometer&  GetTotalUpTimeChronometer() const { return *(_chronometers.find(std::string("Total Up-Time"))->second); }
		Chronometer&  GetUpdateChronometer() const { return *(_chronometers.find(std::string("Update"))->second); }
		Chronometer&  GetRenderChronometer() const { return *(_chronometers.find(std::string("Render"))->second); }
		Chronometer&  GetFrameChronometer() const { return *(_chronometers.find(std::string("Frame"))->second); }
		Chronometer&  GetIdleChronometer() const { return *(_chronometers.find(std::string("Idle"))->second); }
		Chronometer&  GetStatisticsChronometer() const { return *(_chronometers.find(std::string("Statistics"))->second); }
		Chronometer&  GetWindowsMessageChronometer() const { return *(_chronometers.find(std::string("Windows Message"))->second); }
		Chronometer&  GetInputChronometer() const { return *(_chronometers.find(std::string("Input"))->second); }
		Chronometer&  GetTickChronometer() const { return *(_chronometers.find(std::string("Tick"))->second); }
		Chronometer&  GetInBetweenTicksChronometer() const { return *(_chronometers.find(std::string("In Between Ticks"))->second); }
		
		bool IsUpdatePaused() const { return _is_paused; }
		//bool IsRenderPaused() const { return _is_render_paused; }
	
	private:
		void _DeleteAllChronometers();
		void _AddEngineChronometers();
		void _StartAllChronometers();
		void _AddEngineAlarms();
		
		void _SetShouldUpdate();
		void _SetShouldRender();
		void _SetEndOfFrame();
		
		AlarmManager						_alarm_manager;
		std::map<std::string, Chronometer*>	_chronometers;

		Duration	_tgt_update_interval_ns;
		Duration	_tgt_render_interval_ns;
		Duration	_frame_interval;
		Duration	_command_list_interval;
	
		TimePoint curr_tick_time = TimePoint(0ns);
		TimePoint prev_tick_time = TimePoint(0ns);
	
		Duration tick_delta_time_ns	 = 0ns;

		bool _is_paused;

		bool _should_update;
		bool _should_render;
		bool _end_of_frame;
	};
}
