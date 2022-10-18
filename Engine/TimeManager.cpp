module TimeManager;

using namespace mt::time;

void TimeManager::Tick()
{
	auto now = Clock::now();

	prev_tick_time = curr_tick_time;

	curr_tick_time = Clock::now();

	tick_delta_time_ns = curr_tick_time - prev_tick_time;

	_alarm_manager.Tick(curr_tick_time, prev_tick_time, tick_delta_time_ns);

	now = Clock::now();
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

		for (auto& pair : _stop_watches)
		{
			StopWatch*& timer = pair.second;
			timer->continue_task(continue_time);
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

		for (auto& pair : _stop_watches)
		{
			StopWatch*& chrono = pair.second;
			chrono->pause_task(time_paused);
		}
	}

	// else do nothing
}

void TimeManager::_DeleteAllChronometers()
{
	for (auto& pair : _stop_watches)
	{
		StopWatch*& timer = pair.second;
		delete timer;
	}

	_stop_watches.clear();
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