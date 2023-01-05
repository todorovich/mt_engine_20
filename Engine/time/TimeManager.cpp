module TimeManager;

import <chrono>;

import Engine;

using namespace mt::time;

void TimeManager::tick() noexcept
{
	//auto now = std::chrono::steady_clock::now();

	prev_tick_time = curr_tick_time;

	curr_tick_time = std::chrono::steady_clock::now();

	tick_delta_time_ns = curr_tick_time - prev_tick_time;

	_alarm_manager.tick(curr_tick_time);

	//now = std::chrono::steady_clock::now();
}

void TimeManager::updateComplete() noexcept
{
	_should_update = false;
}

void TimeManager::renderComplete() noexcept
{
	_should_render = false;
}

void TimeManager::frameComplete() noexcept
{
	_end_of_frame = false;
}

void TimeManager::resume() noexcept
{
	// If the game is paused, unpause it.
	if (_is_paused)
	{
		_is_paused = false;

		auto continue_time = std::chrono::steady_clock::now();

		_alarm_manager.resume(continue_time);

		for (auto& pair : _stop_watches)
		{
			StopWatch* timer = pair.second.get();
			timer->continueTask(continue_time);
		}
	}

	// else do nothing
}

void TimeManager::pause() noexcept
{
	// If the game is not paused, pause it.
	if (_is_paused == false)
	{
		_is_paused = true;

		auto time_paused = std::chrono::steady_clock::now();

		_alarm_manager.pause(time_paused);

		for (auto& pair : _stop_watches)
		{
			StopWatch* chrono = pair.second.get();
			chrono->pauseTask(time_paused);
		}
	}

	// else do nothing
}

void TimeManager::_addEngineAlarms() noexcept
{
	_alarm_manager.addAlarm(
		std::chrono::steady_clock::now() + _tgt_update_interval_ns,
		[](mt::Engine& engine) noexcept -> void { engine.getTimeManager()->_setShouldUpdate(); },
		true,
		_tgt_update_interval_ns
	);
	_alarm_manager.addAlarm(
		std::chrono::steady_clock::now() + _tgt_render_interval_ns,
		[](mt::Engine& engine) noexcept -> void { engine.getTimeManager()->_setShouldRender(); },
		true,
		_tgt_render_interval_ns
	);
	_alarm_manager.addAlarm(
		std::chrono::steady_clock::now() + _frame_interval,
		[](mt::Engine& engine) noexcept -> void { engine.getTimeManager()->_setEndOfFrame(); },
		true,
		_frame_interval
	);
}

void TimeManager::_setShouldUpdate() noexcept
{
	_should_update = true;
}

void TimeManager::_setShouldRender() noexcept
{
	_should_render = true;
}

void TimeManager::_setEndOfFrame() noexcept
{
	_end_of_frame = true;
}