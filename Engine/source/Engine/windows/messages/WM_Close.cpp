// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Close;

import Engine;

LRESULT mt::windows::WM_Close::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_Close\n");

	// this is where orderly requests to quit come from

	// Confirm the user wants to close.

	static const bool user_wants_to_close = true;

	if (user_wants_to_close)
	{
		// Shutdown the Game
		_engine.shutdown();
	}

	return 0;
}
