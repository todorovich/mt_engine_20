// Copyright 2023 Micho Todorovich, all rights reserved.
module Engine;

import std;

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

const DWORD MS_VC_EXCEPTION = 0x406D1388;

using namespace mt;
using namespace mt::memory;
using namespace mt::error;

Engine* Engine::_instance = nullptr;
std::atomic<bool> Engine::_is_shutting_down = false;
std::atomic<bool> Engine::_should_shut_down = false;

Engine::Engine() noexcept
{
	if (_instance != nullptr)
	{
		Assign(_error, mt::error::ErrorCode::ONE_ENGINE_RULE_VIOLATED);

		crash(_error);
		return;
	}
	else
		_instance = this;
	
	if (_input_manager = make_unique_nothrow<input::BasicInputManager>(*this, _error);
		_input_manager.get() == nullptr || _error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)
	)
	{
		if (_input_manager.get() == nullptr)
			crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
		else
			crash(_error);

		return;
	}

	if (_time_manager = make_unique_nothrow<time::StandardTimeManager>(*this, _error);
		_time_manager.get() == nullptr || _error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)
	)
	{
		if (_time_manager.get() == nullptr)
			crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
		else
			crash(_error);

		return;
	}

	if (_window_manager = make_unique_nothrow<windows::WindowsWindowManager>(
			*this, _error, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)
		);
		_window_manager.get() == nullptr || _error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)
	)
	{
		if (_window_manager.get() == nullptr)
			crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
		else
			crash(_error);

		return;
	}

	// TODO: this should be taking in the error and returning error information if it fails.
	if (_renderer = make_unique_nothrow<renderer::DirectXRenderer>(*this);
		_renderer.get() == nullptr || _error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)
	)
	{
		if (_renderer.get() == nullptr)
			crash(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
		else
			crash(_error);
		
		return;
	}
}

Engine::~Engine() noexcept
{
	_instance = nullptr;

	OutputDebugString(L"Engine Shutdown\n");
}

std::expected<void, std::error_condition> Engine::run(std::unique_ptr<Game> game) noexcept
{
	if (!game) {
		crash(MakeErrorCondition(mt::error::ErrorCode::INVALID_GAME_PROVIDED));
		return std::unexpected{ _error };
	}
	else
		_game = std::move(game);

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

	auto tick_thread = std::jthread([&](){
		if (FAILED(SetThreadDescription(GetCurrentThread(),L"mt::Engine Tick Thread")))
			OutputDebugString(L"failed to set engine tick thread name.");

		while(getWindowManager()->isMessageLoopRunning())
		{
			_time_manager->tick();
		}
	});

	auto expected = getWindowManager()->runMessageLoop();

	run_time->finishTask();

	if (_error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED))
		return std::unexpected{ std::move(_error) };
	else
		return {};
}

void Engine::crash(std::error_condition error) noexcept
{
	// TODO: what if there is already an error?
	//  This is where special pool of errors comes in handy. Could link list errors as well as allow cause.
	// 	then link together unique_ptr's that will clean themselves up.
	
	_error = std::move(error);
	
	//if constexpr (IS_DEBUG) OutputDebugString(_error->message().c_str());
	// TODO: make this print the message? or log it at least.
	shutdown();
};
