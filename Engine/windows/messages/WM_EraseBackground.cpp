// Copyright 2022 Micho Todorovich, all rights reserved.
module WindowsMessages.EraseBackground;

import Engine;

import Windows;

using namespace windows;

using namespace mt::windows;

LRESULT WM_EraseBackground::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
