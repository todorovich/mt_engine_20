module TimeManager;

import Time;
import Timer;
import AlarmManager;
import Chronometer;

import std.core;

using namespace std::literals::chrono_literals;

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
	GetRenderChronometer().Pause();
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
	auto chronometer = new Chronometer(*this, "Total Up-Time", false);
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer(*this, "Update", true, true);
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));
	
	chronometer = new Chronometer(*this, "Render", true, true);
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer(*this, "Statistics", true, true);
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer(*this, "Windows Message", true, true);
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer(*this, "Input", true, true);
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer(*this, "Tick");
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));
	
	chronometer = new Chronometer(*this, "Frame");
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer(*this, "Idle");
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer(*this, "In Between Ticks");
	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

}

void TimeManager::_StartAllChronometers()
{
	for (auto& pair : _chronometers)
	{
		Chronometer*& chronometer = pair.second;
		chronometer->Start();
	}
}

void TimeManager::_StopAllChronometers()
{
	for (auto& pair : _chronometers)
	{
		Chronometer*& chronometer = pair.second;
		chronometer->Stop();
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