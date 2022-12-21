// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.ExitSizeMove;

import Engine;
import TimeManager;
import DirectXRenderer;
import WindowManager;

using namespace mt::windows;

LRESULT WM_ExitSizeMove::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	_engine.getWindowManager()->resize(
		_engine.getRenderer()->getWindowWidth(), _engine.getRenderer()->getWindowHeight()
	);
	
	_engine.getTimeManager()->resume(); // why?
	
	_engine.getWindowManager()->setIsWindowResizing(false);
	
	return 0;
}