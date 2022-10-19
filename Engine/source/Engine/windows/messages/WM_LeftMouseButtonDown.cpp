// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.LeftMouseButtonDown;

import Engine;
import InputManager;

LRESULT mt::windows::WM_LeftMouseButtonDown::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	//MK_SHIFT and MK_CONTROL exist

	_input_manager->mouseEvent(
		GET_X_LPARAM(lParam), 
		GET_Y_LPARAM(lParam),
		wParam & MK_LBUTTON, 
		wParam & MK_RBUTTON, 
		wParam & MK_MBUTTON, 
		wParam & MK_XBUTTON1,
		wParam & MK_XBUTTON2
	);

	return 0;
}
