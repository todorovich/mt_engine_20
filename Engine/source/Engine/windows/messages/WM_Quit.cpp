// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Quit;

import Engine;

LRESULT mt::windows::WM_Quit::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	// This should be the final windows message received as this causes the windows message loop to stop
	// and the main engine tick thread to be joined

	// Command doesn't actually ever get called.

	//OutputDebugStringW(L"WM_Quit\n");

	return 0;
}
