module StandardTimeManager;

import std;

import Error;
import StandardAlarmManager;
import TimeManagerTasks;
import TimeModel;

using namespace mt::error;
using namespace mt::time;
using namespace mt::time::model;

StandardTimeManager::StandardTimeManager(mt::Engine& engine, std::error_condition& _alarm_manager_error) noexcept
	: _alarm_manager(std::make_unique<StandardAlarmManager>(_alarm_manager_error))
	, _stop_watches(_getStopWatches())
	, _engine(engine)
	, _set_should_update(mt::time::TimeManagerSetShouldUpdate(engine))
	, _set_should_render(mt::time::TimeManagerSetShouldRender(engine))
	, _set_end_of_frame(mt::time::TimeManagerSetEndOfFrame(engine))
	, _standard_tick_function(
		findStopWatch(DefaultTimers::TICK_TIME),
		findStopWatch(DefaultTimers::UPDATE_TIME),
		findStopWatch(DefaultTimers::RENDER_TIME),
		findStopWatch(DefaultTimers::FRAME_TIME),
		findStopWatch(DefaultTimers::INPUT_TIME),
		&engine
	)
	, _shutting_down_tick_function(&engine, findStopWatch(DefaultTimers::INPUT_TIME))
	, _initiate_shut_down_tick_function(&engine, _shutting_down_tick_function)
{
	_addEngineAlarms();

	_setCurrentTickTime();
	_setPreviousTickTime(std::chrono::steady_clock::time_point::min());

	_setTickDeltaTime(std::chrono::steady_clock::duration::min());

	_setIsUpdatePaused(false);

	setTickFunction(&_standard_tick_function);
}

//constexpr std::map<std::string_view, std::unique_ptr<Stopwatch>> StandardTimeManager::_getStopWatches()

void StandardTimeManager::tick() noexcept
{
	_setPreviousTickTime(getCurrentTickTime());

	_setCurrentTickTime();

	_setTickDeltaTime(getCurrentTickTime() - getPreviousTickTime());

	_alarm_manager->tick(getCurrentTickTime());

	TimeManagerInterface::tick();
}

void StandardTimeManager::resume() noexcept
{
	// If the game is paused, unpause it.
	if (isUpdatePaused())
	{
		_setIsUpdatePaused(false);

		auto continue_time = std::chrono::steady_clock::now();

		_alarm_manager->resume(continue_time);

		for (auto& pair : _stop_watches)
		{
			StopWatch* timer = pair.second.get();
			timer->continueTask(continue_time);
		}
	}
}

void StandardTimeManager::pause() noexcept
{
	// If the game is not paused, pause it.
	if (isUpdatePaused())
	{
		_setIsUpdatePaused();

		auto time_paused = std::chrono::steady_clock::now();

		_alarm_manager->pause(time_paused);

		for (auto& pair : _stop_watches)
		{
			StopWatch* chrono = pair.second.get();
			chrono->pauseTask(time_paused);
		}
	}

	// else do nothing
}

void StandardTimeManager::_addEngineAlarms() noexcept
{
	auto update_interval = getUpdateInterval();
	_alarm_manager->addAlarm(
		std::chrono::steady_clock::now() + update_interval,
		&_set_should_update,
		true,
		update_interval
	);

	auto render_interval = getRenderInterval();
	_alarm_manager->addAlarm(
		std::chrono::steady_clock::now() + render_interval,
		&_set_should_render,
		true,
		render_interval
	);

	auto frame_interval = getFrameInterval();
	_alarm_manager->addAlarm(
		std::chrono::steady_clock::now() + frame_interval,
		&_set_end_of_frame,
		true,
		frame_interval
	);
}

