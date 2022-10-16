// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.MouseMove;

import Engine;

LRESULT mt::windows::WM_MouseMove::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	mt::Engine::GetInputManager().MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0; // If an application processes this message, it should return zero.
}
