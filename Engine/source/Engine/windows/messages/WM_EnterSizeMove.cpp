// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.EnterSizeMove;

import Engine;
import TimeManager;
import WindowManager;

using namespace mt::windows;

LRESULT WM_EnterSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	_engine.getWindowManager()->setIsWindowResizing(true);

	_engine.getTimeManager()->pause();

	return 0;
}

