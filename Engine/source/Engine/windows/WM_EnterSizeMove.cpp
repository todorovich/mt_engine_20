// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.EnterSizeMove;

import Engine;

using namespace mt::windows;

LRESULT WM_EnterSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& Engine = Engine::GetEngine();

	Engine::SetIsWindowResizing(true);

	Engine.GetTimeManager().Pause();

	return 0;
}

