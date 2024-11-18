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

Engine::Engine() noexcept
{
	if (_instance != nullptr)
	{
		Assign(*_error, mt::error::ErrorCode::ONE_ENGINE_RULE_VIOLATED);

		return;
	}
	else
		_instance = this;

	if (_error == nullptr) return;

	if (_input_manager = make_unique_nothrow<input::BasicInputManager>(*this, *_error);
		_input_manager.get() == nullptr
	)
	{
		if (_input_manager.get() == nullptr)
			Assign(*_error, mt::error::ErrorCode::BAD_ALLOCATION);

		return;
	}

	if (_time_manager = make_unique_nothrow<time::StandardTimeManager>(*this, *_error);
		_time_manager.get() == nullptr || _error->value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)
	)
	{
		if (_time_manager.get() == nullptr)
			Assign(*_error, mt::error::ErrorCode::BAD_ALLOCATION);

		return;
	}

	if (_window_manager = make_unique_nothrow<windows::WindowsWindowManager>(
			*this, *_error, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)
		);
		_window_manager.get() == nullptr || _error->value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)
	)
	{
		if (_window_manager.get() == nullptr) Assign(*_error, mt::error::ErrorCode::BAD_ALLOCATION);

		return;
	}

	// TODO: this should be taking in the error and returning error information if it fails.
	if (_renderer = make_unique_nothrow<renderer::DirectXRenderer>(*this);
		_renderer.get() == nullptr //|| _error->getErrorCode() != mt::error::ErrorCode::ERROR_UNINITIALIZED
	)
	{
		Assign(*_error, mt::error::ErrorCode::BAD_ALLOCATION);

		return;
	}
}

Engine::~Engine() noexcept
{
	_instance = nullptr;

	OutputDebugString(L"Engine Shutdown\n");
}

std::expected<void, std::unique_ptr<std::error_condition>> Engine::run(std::unique_ptr<Game> game) noexcept
{
	if (_error == nullptr) return std::unexpected(std::move(_error));

	if (!game) {
		Assign(*_error, ErrorCode::INVALID_GAME_PROVIDED);
		return std::unexpected{std::move(_error)};
	}
	else
		_game = std::move(game);

	if (_error->value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED))
		return std::unexpected(std::move(_error));

	if (auto expected = getWindowManager()->createMainWindow(); !expected)
	{
		(*_error) = std::move(expected.error());
		return std::unexpected{std::move(_error)};
	}

	if (auto expected = getRenderer()->initialize(); !expected)
	{
		(*_error) = std::move(expected.error());
		return std::unexpected{std::move(_error)};
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

	if (_error->value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED))
		return std::unexpected{ std::move(_error) };
	else
		return {};
}

void Engine::crash(std::error_condition error) noexcept
{
	// TODO: what if there is already an error?
	//  This is where special pool of errors comes in handy. Could link list errors as well as allow cause.
	// 	then link together unique_ptr's that will clean themselves up.
	(*_error) = std::move(error);
	//if constexpr (IS_DEBUG) OutputDebugString(_error->message().data());
	// TODO: make this print the message? or log it at least.
	shutdown();
};
