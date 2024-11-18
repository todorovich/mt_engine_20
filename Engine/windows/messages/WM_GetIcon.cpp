// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.GetIcon;

import Engine;

import Windows;

using namespace windows;

LRESULT mt::windows::WM_GetIcon::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
