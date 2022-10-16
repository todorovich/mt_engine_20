// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.ExitSizeMove;

import Engine;

using namespace mt::windows;

LRESULT WM_ExitSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& Engine = Engine::GetEngine();

	Engine::Resize(Engine.GetWindowWidth(), Engine.GetWindowHeight());
	
	Engine::GetTimeManager().Continue();
	
	Engine::SetIsWindowResizing(false);
	
	return 0;
}