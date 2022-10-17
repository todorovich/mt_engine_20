// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Size;

import Engine;
import DirectXRenderer;
import Camera;
import TimeManager;
import std.core;

LRESULT mt::windows::WM_Size::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	// Save the new client area dimensions.
	auto _window_width = LOWORD(lParam);
	auto _window_height = HIWORD(lParam);

	_engine.Resize(_window_width, _window_height);

	if (_engine.GetRenderer()->GetIsInitialized())
	{
		if (wParam == SIZE_MINIMIZED)
		{
			_engine.GetTimeManager()->Pause();
			_engine.SetIsWindowMinimized(true);
			_engine.SetIsWindowMaximized(false);
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			_engine.GetTimeManager()->Continue();
			_engine.SetIsWindowMinimized(false);
			_engine.SetIsWindowMaximized(true);
			_engine.Resize(_window_width, _window_height);
		}
		else if (wParam == SIZE_RESTORED)
		{
			// Restoring from minimized state?
			if (_engine.IsWindowMinimized())
			{
				_engine.GetTimeManager()->Continue();
				_engine.SetIsWindowMinimized(false);
				_engine.Resize(_window_width, _window_height);
			}

			// Restoring from maximized state?
			else if (_engine.IsWindowMaximized())
			{
				_engine.GetTimeManager()->Continue();
				_engine.SetIsWindowMinimized(false);
				_engine.Resize(_window_width, _window_height);
			}

			else if (_engine.IsWindowResizing())
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
				_engine.Resize(_window_width, _window_height);
			}
		}
	}

	return 0;
}

