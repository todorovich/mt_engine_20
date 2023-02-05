// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsWindowManager;

import Engine;

import WindowsMessageManager;

using namespace mt::windows;

using namespace std::literals;

std::expected<void, mt::error::Error> WindowsWindowManager::initialize() noexcept
{
	_window_class.style = CS_HREDRAW | CS_VREDRAW;
	_window_class.lpfnWndProc = WindowsMessageManagerInterface::MainWndProc;
	_window_class.cbClsExtra = 0;
	_window_class.cbWndExtra = 0;
	_window_class.hInstance = _instance_handle;
	_window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
	_window_class.hCursor = LoadCursor(0, IDC_ARROW);
	_window_class.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	_window_class.lpszMenuName = 0;
	_window_class.lpszClassName = L"MainWnd";

	if (!RegisterClass(&_window_class))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return std::unexpected(mt::error::Error{
			L"Unable to register the class with windows."sv,
			mt::error::ErrorCode::WINDOW_MANAGER_FAILURE,
			__func__, __FILE__, __LINE__
		});
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT rectangle = { 0, 0, getWindowWidth(), getWindowHeight() };
	AdjustWindowRect(&rectangle, WS_OVERLAPPEDWINDOW, false);
	int width = rectangle.right - rectangle.left;
	int height = rectangle.bottom - rectangle.top;

	_main_window_handle = CreateWindow(
		L"MainWnd", _main_window_caption.c_str(), WS_MAXIMIZE, 0, 0,
		width, height, nullptr, nullptr, _instance_handle, 0
	);

	if (!_main_window_handle)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return std::unexpected(mt::error::Error{
			L"Unable to create the window."sv,
			mt::error::ErrorCode::WINDOW_MANAGER_FAILURE,
			__func__, __FILE__, __LINE__
		});
	}

	SetWindowLong(_main_window_handle, GWL_STYLE, 0); //remove all window styles, check MSDN for details

	ShowWindow(_main_window_handle, SW_SHOWMAXIMIZED); //display window

	UpdateWindow(_main_window_handle);

	return {};
}

std::expected<void, mt::error::Error> WindowsWindowManager::shutdown() noexcept
{
	DestroyWindow(static_cast<HWND>(getMainWindowHandle()));

	return {};
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
