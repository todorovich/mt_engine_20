// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Size;

import Engine;
import WindowManagerInterface;
import Camera;

LRESULT mt::windows::WM_Size::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
)
{
	// Save the new client area dimensions.
	auto _window_width = LOWORD(lParam);
	auto _window_height = HIWORD(lParam);

	mt::renderer::RendererInterface* renderer = _engine->getRenderer();
	mt::time::TimeManagerInterface& time_manager = *_engine->getTimeManager();
	WindowManagerInterface& window_manager = *_engine->getWindowManager();

	window_manager.resize(_window_width, _window_height);

	if (renderer->getIsInitialized())
	{
		if (wParam == SIZE_MINIMIZED)
		{
			time_manager.pause();
			window_manager.setIsWindowMinimized(true);
			window_manager.setIsWindowMaximized(false);
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			time_manager.resume();
			window_manager.setIsWindowMinimized(false);
			window_manager.setIsWindowMaximized(true);
			window_manager.resize(_window_width, _window_height);
		}
		else if (wParam == SIZE_RESTORED)
		{
			// Restoring from minimized state?
			if (window_manager.isWindowMinimized())
			{
				time_manager.resume();
				window_manager.setIsWindowMinimized(false);
				window_manager.resize(_window_width, _window_height);
			}

			// Restoring from maximized state?
			else if (window_manager.isWindowMaximized())
			{
				time_manager.resume();
				window_manager.setIsWindowMinimized(false);
				window_manager.resize(_window_width, _window_height);
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
				window_manager.resize(_window_width, _window_height);
			}
		}
	}

	return 0;
}

