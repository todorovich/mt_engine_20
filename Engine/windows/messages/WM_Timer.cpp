// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Timer;

import Engine;

LRESULT mt::windows::WM_Timer::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
