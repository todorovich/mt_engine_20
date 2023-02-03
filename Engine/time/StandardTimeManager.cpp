module StandardTimeManager;

import <chrono>;

import StandardAlarmManager;
import TimeManagerTasks;

//using namespace gsl;
using namespace mt::time;

StandardTimeManager::StandardTimeManager(mt::Engine* engine) noexcept
	: _alarm_manager(std::make_unique<StandardAlarmManager>())
	, _stop_watches()
	, _set_should_update(mt::time::TimeManagerSetShouldUpdate{engine})
	, _set_should_render(mt::time::TimeManagerSetShouldRender(engine))
	, _set_end_of_frame(mt::time::TimeManagerSetEndOfFrame(engine))
{
	_stop_watches.emplace(std::make_pair(DefaultTimers::RUN_TIME,				std::make_unique<StopWatch>(DefaultTimers::RUN_TIME)));
	_stop_watches.emplace(std::make_pair(DefaultTimers::WINDOWS_MESSAGE_TIME,	std::make_unique<StopWatch>(DefaultTimers::WINDOWS_MESSAGE_TIME)));
	_stop_watches.emplace(std::make_pair(DefaultTimers::TICK_TIME,				std::make_unique<StopWatch>(DefaultTimers::TICK_TIME)));
	_stop_watches.emplace(std::make_pair(DefaultTimers::UPDATE_TIME,			std::make_unique<StopWatch>(DefaultTimers::UPDATE_TIME)));
	_stop_watches.emplace(std::make_pair(DefaultTimers::INPUT_TIME,				std::make_unique<StopWatch>(DefaultTimers::INPUT_TIME)));
	_stop_watches.emplace(std::make_pair(DefaultTimers::RENDER_TIME,			std::make_unique<StopWatch>(DefaultTimers::RENDER_TIME)));
	_stop_watches.emplace(std::make_pair(DefaultTimers::FRAME_TIME,				std::make_unique<StopWatch>(DefaultTimers::FRAME_TIME)));

	_addEngineAlarms();

	_setCurrentTickTime();
	_setPreviousTickTime(std::chrono::steady_clock::time_point::min());

	_setTickDeltaTime(std::chrono::steady_clock::duration::min());

	_setIsUpdatePaused(false);
}

void StandardTimeManager::tick() noexcept
{
	//auto now = std::chrono::steady_clock::now();

	_setPreviousTickTime(getCurrentTickTime());

	_setCurrentTickTime();

	_setTickDeltaTime(getCurrentTickTime() - getPreviousTickTime());

	_alarm_manager->tick(getCurrentTickTime());

	//now = std::chrono::steady_clock::now();
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

