// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsWindowManager;

import Engine;

import WindowsMessageManager;

import MakeUnique;

using namespace mt::error;
using namespace mt::memory;
using namespace mt::windows;

using namespace std::literals;
// TODO: init with a matching shutdown seems like an object to me
std::expected<void, mt::error::Error> WindowsWindowManager::createMainWindow() noexcept
{
	Error error;
	_window = make_unique_nothrow<Window>(error, getWindowWidth(), getWindowHeight(), _instance_handle);

	if (_window.get() == nullptr)
		return std::unexpected(mt::error::Error{
			L"Unable to create the main window."sv,
			mt::error::ErrorCode::WINDOW_MANAGER_FAILURE,
			__func__, __FILE__, __LINE__
		});

	if (error.getErrorCode() != ErrorCode::ERROR_UNINITIALIZED)
		return std::unexpected{error};
	else
		return {};
}


std::expected<void, mt::error::Error> WindowsWindowManager::runMessageLoop() noexcept
{
	return (*(_windows_message_manager->getMessageLoopTask()))();
}

std::expected<void, mt::error::Error> WindowsWindowManager::destroyMainWindow() noexcept
{
	_windows_message_manager->destroyMainWindow();

	return {};
}

bool WindowsWindowManager::isMessageLoopRunning() noexcept
{
	return !_windows_message_manager->hasReceivedQuit();
}

void WindowsWindowManager::toggleShowCursor() noexcept
{
	_windows_message_manager->toggleShowCursor();
}

std::expected<void, mt::error::Error> WindowsWindowManager::resize(int width, int height) noexcept
{
	if (auto expected = mt::windows::WindowManagerInterface::resize(width, height); !expected)
		return std::unexpected(expected.error());

	// This flag should prevent further rendering after the current frame finishes
	setIsWindowResizing(true);

	auto renderer = _engine.getRenderer();

	// TODO: don't wait.
	// wait until rendering is finished.
	while (renderer->getIsRendering()) {};

	// TODO: Observer Pattern?
	if(renderer->getIsInitialized())
	{
		if (auto expected = renderer->onResize(); !expected)
			return std::unexpected(expected.error());
	}

	// Continue rendering.
	setIsWindowResizing(false);

	return {};
}
