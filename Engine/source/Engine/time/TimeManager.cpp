module TimeManager;

import Engine;

using namespace mt::time;

void TimeManager::tick()
{
	auto now = Clock::now();

	prev_tick_time = curr_tick_time;

	curr_tick_time = Clock::now();

	tick_delta_time_ns = curr_tick_time - prev_tick_time;

	_alarm_manager.tick(curr_tick_time, prev_tick_time, tick_delta_time_ns);

	now = Clock::now();
}

void TimeManager::updateComplete()
{
	_should_update = false;
}

void TimeManager::renderComplete()
{
	_should_render = false;
}

void TimeManager::frameComplete()
{
	_end_of_frame = false;
}

void TimeManager::resume()
{
	// If the game is paused, unpause it.
	if (_is_paused)
	{
		_is_paused = false;

		auto continue_time = Clock::now();

		_alarm_manager.resume(continue_time);

		for (auto& pair : _stop_watches)
		{
			StopWatch* timer = pair.second.get();
			timer->continueTask(continue_time);
		}
	}

	// else do nothing
}

void TimeManager::pause()
{
	// If the game is not paused, pause it.
	if (_is_paused == false)
	{
		_is_paused = true;

		auto time_paused = Clock::now();

		_alarm_manager.pause(time_paused);

		for (auto& pair : _stop_watches)
		{
			StopWatch* chrono = pair.second.get();
			chrono->pauseTask(time_paused);
		}
	}

	// else do nothing
}

void TimeManager::_addEngineAlarms()
{
	_alarm_manager.addAlarm(Clock::now() + _tgt_update_interval_ns, [](mt::Engine& engine) -> void { engine.getTimeManager()->_setShouldUpdate(); }, true, _tgt_update_interval_ns);
	_alarm_manager.addAlarm(Clock::now() + _tgt_render_interval_ns, [](mt::Engine& engine) -> void { engine.getTimeManager()->_setShouldRender(); }, true, _tgt_render_interval_ns);
	_alarm_manager.addAlarm(Clock::now() + _frame_interval, [](mt::Engine& engine) -> void { engine.getTimeManager()->_setEndOfFrame(); }, true, _frame_interval);
}

void TimeManager::_setShouldUpdate()
{
	_should_update = true;
}

void TimeManager::_setShouldRender()
{
	_should_render = true;
}

void TimeManager::_setEndOfFrame()
{
	_end_of_frame = true;
}