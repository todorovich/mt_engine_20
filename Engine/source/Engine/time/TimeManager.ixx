// Copyright 2022 Micho Todorovich, all rights reserved.
export module TimeManager;

export import Time;
export import Timer;
export import AlarmManager;
export import Chronometer;

import std.core;

using namespace std::literals::chrono_literals;

export namespace mt::time
{
	class TimeManager
	{
		static const int		_number_of_samples_to_use = 100;
	
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
		std::map<std::string, Chronometer*>		_chronometers;

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

using namespace mt::time;

void TimeManager::Initialize()
{
	_DeleteAllChronometers();

	_AddEngineChronometers();

	_StartAllChronometers();

	_AddEngineAlarms();

	curr_tick_time = Clock::now();
	prev_tick_time = TimePoint(0ns);

	tick_delta_time_ns = 0ns;

	_is_paused = false;
}

void TimeManager::Tick()
{
	auto now = Clock::now();
	GetIdleChronometer().Pause(now);
	GetTickChronometer().Continue(now);

	prev_tick_time = curr_tick_time;

	curr_tick_time = Clock::now();

	tick_delta_time_ns = curr_tick_time - prev_tick_time;

	_alarm_manager.Tick(curr_tick_time, prev_tick_time, tick_delta_time_ns);

	// Tick all the chronometers
	for (auto& chronometer : _chronometers)
	{
		chronometer.second->Tick(curr_tick_time, prev_tick_time, tick_delta_time_ns);
	}

	now = Clock::now();
	GetTickChronometer().Pause(now);
	GetIdleChronometer().Continue(now);
}

void TimeManager::UpdateComplete()
{
	_should_update = false;
}

void TimeManager::RenderComplete()
{
	_should_render = false;
}

void TimeManager::FrameComplete()
{
	_end_of_frame = false;
}

void TimeManager::Continue()
{
	// If the game is paused, unpause it.
	if (_is_paused)
	{
		_is_paused = false;

		auto continue_time = Clock::now();

		_alarm_manager.Continue(continue_time);

		for (auto& pair : _chronometers)
		{
			Chronometer*& timer = pair.second;
			timer->Continue(continue_time);
		}
	}

	// else do nothing
}

void TimeManager::Pause()
{
	// If the game is not paused, pause it.
	if (_is_paused == false)
	{
		_is_paused = true;

		auto time_paused = Clock::now();

		_alarm_manager.Pause(time_paused);

		for (auto& pair : _chronometers)
		{
			Chronometer*& chrono = pair.second;
			chrono->Pause(time_paused);
		}
	}

	// else do nothing
}

void TimeManager::_DeleteAllChronometers()
{
	for (auto& pair : _chronometers)
	{
		Chronometer*& timer = pair.second;
		delete timer;
	}

	_chronometers.clear();
}

void TimeManager::_AddEngineChronometers()
{
	auto chronometer = new Chronometer("Total Up-Time");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Update");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Render");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Statistics");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Frame");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Idle");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Windows Message");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Input");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Tick");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("In Between Ticks");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

}

void TimeManager::_StartAllChronometers()
{
	for (auto& pair : _chronometers)
	{
		Chronometer*& timer = pair.second;
		timer->Start();
	}
}

void TimeManager::_AddEngineAlarms()
{
	_alarm_manager.AddAlarm(Clock::now() + _tgt_update_interval_ns, [&]() -> void { _SetShouldUpdate(); }, true, _tgt_update_interval_ns);
	_alarm_manager.AddAlarm(Clock::now() + _tgt_render_interval_ns, [&]() -> void { _SetShouldRender(); }, true, _tgt_render_interval_ns);
	_alarm_manager.AddAlarm(Clock::now() + _frame_interval, [&]() -> void { _SetEndOfFrame(); }, true, _frame_interval);
}

void TimeManager::_SetShouldUpdate()
{
	_should_update = true;
}

void TimeManager::_SetShouldRender()
{
	_should_render = true;
}

void TimeManager::_SetEndOfFrame()
{
	_end_of_frame = true;
}