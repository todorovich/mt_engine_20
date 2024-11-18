// Copyright 2022 Micho Todorovich, all rights reserved.
module WindowsMessages.CaptureChanged;

import Windows;

using namespace windows;

LRESULT mt::windows::WM_CaptureChanged::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
