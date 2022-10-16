// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Size;

import Engine;

LRESULT mt::windows::WM_Size::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	// Save the new client area dimensions.
	auto _window_width = LOWORD(lParam);
	auto _window_height = HIWORD(lParam);

	mt::Engine::GetEngine().SetWindowDimensions(_window_width, _window_height);

	if (mt::Engine::GetRenderer().GetIsInitialized())
	{
		if (wParam == SIZE_MINIMIZED)
		{
			mt::Engine::GetTimeManager().Pause();
			mt::Engine::SetIsWindowMinimized(true);
			mt::Engine::SetIsWindowMaximized(false);
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			mt::Engine::GetTimeManager().Continue();
			mt::Engine::SetIsWindowMinimized(false);
			mt::Engine::SetIsWindowMaximized(true);
			mt::Engine::Resize(_window_width, _window_height);
		}
		else if (wParam == SIZE_RESTORED)
		{
			// Restoring from minimized state?
			if (mt::Engine::GetEngine().IsWindowMinimized())
			{
				mt::Engine::GetTimeManager().Continue();
				mt::Engine::SetIsWindowMinimized(false);
				mt::Engine::Resize(_window_width, _window_height);
			}

			// Restoring from maximized state?
			else if (Engine::GetEngine().IsWindowMaximized())
			{
				mt::Engine::GetTimeManager().Continue();
				mt::Engine::SetIsWindowMinimized(false);
				mt::Engine::Resize(_window_width, _window_height);
			}

			else if (mt::Engine::GetEngine().IsWindowResizing())
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
				mt::Engine::Resize(_window_width, _window_height);
			}
		}
	}

	return 0;
}

