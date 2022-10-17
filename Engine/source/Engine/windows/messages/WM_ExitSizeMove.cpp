// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.ExitSizeMove;

import Engine;
import TimeManager;
import DirectXRenderer;

using namespace mt::windows;

LRESULT WM_ExitSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	_engine.Resize(_engine.GetRenderer()->GetWindowWidth(), _engine.GetRenderer()->GetWindowHeight());
	
	_engine.GetTimeManager()->Continue();
	
	_engine.SetIsWindowResizing(false);
	
	return 0;
}