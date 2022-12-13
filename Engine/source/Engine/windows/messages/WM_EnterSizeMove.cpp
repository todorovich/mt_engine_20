// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.EnterSizeMove;

#pragma warning( push )
#pragma warning( disable : 5050 )
import std.core;
#pragma warning( pop )

import Engine;
import TimeManager;
import WindowManager;

using namespace mt::windows;

LRESULT WM_EnterSizeMove::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	_engine.getWindowManager()->setIsWindowResizing(true);

	_engine.getTimeManager()->pause();

	return 0;
}

