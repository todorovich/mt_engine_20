// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Moving;

import Engine;

LRESULT mt::windows::WM_Moving::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
