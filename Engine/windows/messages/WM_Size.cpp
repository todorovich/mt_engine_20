// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.Size;

import Engine;
import WindowManagerInterface;
import Camera;
import Windows;

using namespace windows;

LRESULT mt::windows::WM_Size::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
)
{
	// Save the new client area dimensions.
	auto _window_width = LOWORD(lParam);
	auto _window_height = HIWORD(lParam);

	mt::time::TimeManagerInterface& time_manager = *_engine->getTimeManager();
	WindowManagerInterface& window_manager = *_engine->getWindowManager();

	if (wParam == SIZE_MINIMIZED)
	{
		time_manager.pause();
		// TODO: extract into method on the window manager, maybe fire an event from the window manager?
		window_manager.setIsWindowMinimized(true);
		window_manager.setIsWindowMaximized(false);
	}

	else if (wParam == SIZE_MAXIMIZED)
	{
		time_manager.resume();
		// TODO: extract into method on the window manager, maybe fire an event from the window manager?
		window_manager.setIsWindowMinimized(false);
		window_manager.setIsWindowMaximized(true);
		if (auto expected = window_manager.resize(_window_width, _window_height); !expected)
			_engine->crash(expected.error());
	}

	else if (wParam == SIZE_RESTORED)
	{
		// Restoring from minimized state?
		if (window_manager.isWindowMinimized())
		{
			time_manager.resume();
			window_manager.setIsWindowMinimized(false);
			if (auto expected = window_manager.resize(_window_width, _window_height); !expected)
				_engine->crash(expected.error());
		}
		// Restoring from maximized state?
		else if (window_manager.isWindowMaximized())
		{
			time_manager.resume();
			window_manager.setIsWindowMinimized(false);
			if (auto expected = window_manager.resize(_window_width, _window_height); !expected)
				_engine->crash(expected.error());
		}
		else if (window_manager.isWindowResizing())
		{
			// If user is dragging the Resize bars, we do not Resize
			// the buffers here because as the user continuously
			// drags the Resize bars, a stream of WM_SIZE messages are
			// sent to the window, and it would be pointless (and slow)
			// to Resize for each WM_SIZE message received from dragging
			// the Resize bars.  So instead, we reset after the user is
			// done resizing the window and releases the Resize bars, which
			// sends a WM_EXITSIZEMOVE message.
		}
		else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
		{
			if (auto expected = window_manager.resize(_window_width, _window_height); !expected)
				_engine->crash(expected.error());
		}
	}

	return 0;
}

