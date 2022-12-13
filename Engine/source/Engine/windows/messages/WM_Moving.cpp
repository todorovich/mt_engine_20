// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Moving;

#pragma warning( push )
#pragma warning( disable : 5050 )
import std.core;
#pragma warning( pop )

import Engine;

LRESULT mt::windows::WM_Moving::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
