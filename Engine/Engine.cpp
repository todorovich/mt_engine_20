// Copyright 2024 Micho Todorovich, all rights reserved.
module Engine;

import std;

import TimeManagerInterface;
import DirectXRenderer;
import BasicInputManager;
import StandardTimeManager;
import WindowsWindowManager;
import WindowsMessageManager;

import Camera;
import StopWatch;
import MakeUnique;
import Windows;

using namespace ::windows;
using namespace std::literals;
using namespace mt;
using namespace mt::memory;
using namespace mt::error;

mt::Engine* mt::Engine::_instance = nullptr;
std::atomic<bool> mt::Engine::_is_shutting_down = false;
std::atomic<bool> mt::Engine::_should_shut_down = false;

Engine::~Engine() noexcept
{
	_instance = nullptr;

	OutputDebugString(L"Engine Shutdown\n");
}

std::expected<void, std::error_condition> Engine::run(std::shared_ptr<mt::Game> game) noexcept
{
	if (!game) {
		crash(MakeErrorCondition(mt::error::ErrorCode::INVALID_GAME_PROVIDED));
		return std::unexpected{ _error };
	}
	else
		setGame(game);

	if (_error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED))
		return std::unexpected{ _error };

	if (auto expected = getWindowManager()->createMainWindow(); !expected)
	{
		_error = std::move(expected.error());
		return std::unexpected{ _error };
	}

	if (auto expected = getRenderer()->initialize(); !expected)
	{
		_error = std::move(expected.error());
		return std::unexpected{ _error };
	}

	auto run_time = getTimeManager()->findStopWatch(mt::time::DefaultTimers::RUN_TIME);
	run_time->startTask();

	auto frame_time = getTimeManager()->findStopWatch(mt::time::DefaultTimers::FRAME_TIME);
	frame_time->startTask();

	auto tick_thread = std::jthread([this](std::stop_token stop_token) {
		if (FAILED(SetThreadDescription(GetCurrentThread(), L"mt::Engine Tick Thread")))
			OutputDebugString(L"failed to set engine tick thread name.");

		while (getWindowManager()->isMessageLoopRunning() && !stop_token.stop_requested())
		{
			auto time = getTimeManager();
			time->tick();
		}
	});

	auto expected = getWindowManager()->runMessageLoop();

	tick_thread.join();

	run_time->finishTask();

	

	if (_error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED))
		return std::unexpected{ std::move(_error) };
	else
		return {};

	
}

std::error_condition Engine::crash(std::error_condition error) noexcept
{
	// TODO: what if there is already an error?
	//  This is where special pool of errors comes in handy. Could link list errors as well as allow cause.
	// 	then link together unique_ptr's that will clean themselves up.
	
	_error = error;

	//if constexpr (IS_DEBUG) OutputDebugString(_error->message().c_str());
	// TODO: make this print the message? or log it at least.
	shutdown();

	return _error;
}
