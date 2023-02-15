// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module Engine;

import <string_view>;
import <chrono>;

import DirectXRenderer;
import BasicInputManager;
import StandardTimeManager;
import WindowsWindowManager;
import WindowsMessageManager;

import Camera;
import StopWatch;
import MakeUnique;

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
		(*_error) = Error(
			L"Only one instance of the engine may exist at a time"sv,
			mt::error::ErrorCode::ONE_ENGINE_RULE_VIOLATED,
			__func__, __FILE__, __LINE__
		);

		return;
	}
	else
		_instance = this;

	if (_error == nullptr) return;

	if (_input_manager = make_unique_nothrow<input::BasicInputManager>(*this, *_error);
		_input_manager.get() == nullptr
	)
	{
		if (_input_manager.get() == nullptr) (*_error) = Error(
			L"Unable to allocate the input manager."sv,
			mt::error::ErrorCode::BAD_ALLOCATION,
			__func__, __FILE__, __LINE__
		);

		return;
	}

	if (_time_manager = make_unique_nothrow<time::StandardTimeManager>(*this, *_error);
		_time_manager.get() == nullptr || _error->getErrorCode() != mt::error::ErrorCode::ERROR_UNINITIALIZED
	)
	{
		if (_time_manager.get() == nullptr) (*_error) = Error(
			L"Unable to allocate the time manager."sv,
			mt::error::ErrorCode::BAD_ALLOCATION,
			__func__, __FILE__, __LINE__
		);

		return;
	}

	if (_window_manager = make_unique_nothrow<windows::WindowsWindowManager>(
			*this, *_error, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)
		);
		_window_manager.get() == nullptr || _error->getErrorCode() != mt::error::ErrorCode::ERROR_UNINITIALIZED
	)
	{
		if (_window_manager.get() == nullptr) (*_error) = Error(
			L"Unable to allocate the window manager."sv,
			mt::error::ErrorCode::BAD_ALLOCATION,
			__func__, __FILE__, __LINE__
		);

		return;
	}

	// TODO: this should be taking in the error and returning error information if it fails.
	if (_renderer = make_unique_nothrow<renderer::DirectXRenderer>(*this);
		_renderer.get() == nullptr //|| _error->getErrorCode() != mt::error::ErrorCode::ERROR_UNINITIALIZED
	)
	{
		(*_error) = Error(
			L"Unable to allocate the renderer."sv,
			mt::error::ErrorCode::BAD_ALLOCATION,
			__func__, __FILE__, __LINE__
		);

		return;
	}

	if (auto expected = getWindowManager()->createMainWindow(); !expected)
	{
		(*_error) = std::move(expected.error());
		return;
	}

	if (auto expected = getRenderer()->initialize(); !expected)
	{
		(*_error) = std::move(expected.error());
		return;
	}
}

Engine::~Engine() noexcept
{
	_instance = nullptr;

	OutputDebugStringW(L"Engine Shutdown\n");
}

std::expected<void, std::unique_ptr<Error>> Engine::run(std::unique_ptr<Game> game) noexcept
{
	if (_error == nullptr) return std::unexpected(std::move(_error));

	if (!game) {
		(*_error) = Error(
			L"The game provided to Engine::run must not be null"sv,
			mt::error::ErrorCode::INVALID_GAME_PROVIDED,
			__func__, __FILE__, __LINE__
		);
		return std::unexpected{std::move(_error)};
	}
	else
		_game = std::move(game);

	if (_error->getErrorCode() != ErrorCode::ERROR_UNINITIALIZED) return std::unexpected(std::move(_error));

	auto run_time = getTimeManager()->findStopWatch(mt::time::DefaultTimers::RUN_TIME);
	run_time->startTask();

	auto frame_time = getTimeManager()->findStopWatch(mt::time::DefaultTimers::FRAME_TIME);
	frame_time->startTask();

	auto tick_thread = std::jthread([&](){
		HRESULT hr = SetThreadDescription(GetCurrentThread(),L"mt::Engine Tick Thread");
		while(getWindowManager()->isMessageLoopRunning())
		{
			_time_manager->tick();
		}
	});

	auto expected = getWindowManager()->runMessageLoop();

	run_time->finishTask();

	if (_error->getErrorCode() != ErrorCode::ERROR_UNINITIALIZED)
		return std::unexpected{ std::move(_error) };
	else
		return {};
}

/**
 * have to flush commands on gpu before destroying the window,
 * destroying the windows causes PostQuitMessage via WM_Destroy
 * The quit message terminates the windows message loop.
 * would like to use raii and handle most of this shutdown stuff in the destructor.
 * the problem is we won't get WM_Destroy, unless we destroy the window.
 * We can't destroy the window while the renderer is using it.
 *
 * If we destroy in the destructor then we will never receive the WM_Destroy and therefore never PostQuitMessage
 *
 *  If we just delay quiting the message loop, then it'll eventually call to an non-existant WindowMessageManager
 *
 */
void Engine::shutdown() noexcept
{
	if (!isShuttingDown())
	{
		_is_shutting_down.store(true);

		getTimeManager()->shutdown();

		if constexpr (IS_DEBUG) OutputDebugStringW(L"Engine Shutdown Initiated\n");

		// This can fail... but we're shutting down either way right?
		//ExitProcess(0); // may need exit process if this fails
		auto expected1 = getRenderer()->shutdown();
		// Destroy the windows.

		auto expected2 = getWindowManager()->destroyMainWindow();

		// This will eventually be called by WM_Destroy after it receives the message from windows that the window was
		// destroyed.
		//PostQuitMessage(0);
	}
	else
	{
		OutputDebugStringW(L"Engine Shutdown Already Initiated\n");
	}
}

void Engine::crash(mt::error::Error error) noexcept
{
	// TODO: what if there is already an error?
	//  This is where special pool of errors comes in handy. Could link list errors as well as allow cause.
	// 	then link together unique_ptr's that will clean themselves up.
	(*_error) = std::move(error);
	if constexpr (IS_DEBUG) OutputDebugStringW(_error->getMessage().data());
	shutdown();
};
