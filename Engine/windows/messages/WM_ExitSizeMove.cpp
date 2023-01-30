// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.ExitSizeMove;

import WindowManager;

using namespace mt::windows;

LRESULT WM_ExitSizeMove::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	// TODO: what to do if this fails?
	auto expected = _engine->getWindowManager()->resize(
		_engine->getRenderer()->getWindowWidth(), _engine->getRenderer()->getWindowHeight()
	);

	_engine->getTimeManager()->resume(); // why?
	
	_engine->getWindowManager()->setIsWindowResizing(false);
	
	return 0;
}