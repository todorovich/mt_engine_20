// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Activate;

import Engine;

LRESULT mt::windows::WM_Activate::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	if (LOWORD(wParam) == WA_INACTIVE)
	{
		mt::Engine::GetTimeManager().Pause();
	}
	else
	{
		mt::Engine::GetTimeManager().Continue();
	}

	return 0;
}

