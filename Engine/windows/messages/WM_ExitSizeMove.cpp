// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.ExitSizeMove;

import Engine;

import Windows;

using namespace windows;

using namespace mt::windows;

// TODO: revisit how the whole resize process is communicated to my window manager.
LRESULT WM_ExitSizeMove::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	auto window_manager =_engine->getWindowManager();
	if (
		auto expected = window_manager->resize(window_manager->getWindowWidth(), window_manager->getWindowHeight());
		!expected
	)
	{
		_engine->crash(expected.error());
	}

	_engine->getTimeManager()->resume(); // why?

	window_manager->setIsWindowResizing(false);
	
	return 0;
}