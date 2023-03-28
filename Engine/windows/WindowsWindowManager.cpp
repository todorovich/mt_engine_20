// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <expected>

module WindowsWindowManager;

import Engine;

import WindowsMessageManager;

import MakeUnique;

using namespace mt::error;
using namespace mt::memory;
using namespace mt::windows;

using namespace std::literals;
// TODO: init with a matching shutdown seems like an object to me
std::expected<void, std::error_condition> WindowsWindowManager::createMainWindow() noexcept
{
	std::error_condition error;
	_window = make_unique_nothrow<Window>(error, getWindowWidth(), getWindowHeight(), _instance_handle);

	if (_window.get() == nullptr)
		return std::unexpected(MakeErrorCondition(ErrorCode::CREATE_MAIN_WINDOW_FAILED));
	else if (error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED))
		return std::unexpected{error};
	else
		return {};
}

std::expected<void, std::error_condition> WindowsWindowManager::runMessageLoop() noexcept
{
	return (*(_windows_message_manager->getMessageLoopTask()))();
}

std::expected<void, std::error_condition> WindowsWindowManager::destroyMainWindow() noexcept
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

std::expected<void, std::error_condition> WindowsWindowManager::resize(int width, int height) noexcept
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
