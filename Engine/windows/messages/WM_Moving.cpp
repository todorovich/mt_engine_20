// Copyright 2018 Micho Todorovich, all rights reserved.
module WindowsMessages.Moving;

import Engine;
import Windows;

using namespace windows;

LRESULT mt::windows::WM_Moving::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
