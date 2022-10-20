// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.ExitSizeMove;

import Engine;
import TimeManager;
import DirectXRenderer;
import WindowManager;

using namespace mt::windows;

LRESULT WM_ExitSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	_engine.GetWindowManager()->resize(
		_engine.GetRenderer()->getWindowWidth(), _engine.GetRenderer()->getWindowHeight()
	);
	
	_engine.GetTimeManager()->resume(); // why?
	
	_engine.GetWindowManager()->setIsWindowResizing(false);
	
	return 0;
}