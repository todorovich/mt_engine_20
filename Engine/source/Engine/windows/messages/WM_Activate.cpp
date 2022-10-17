// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Activate;

import Engine;
import TimeManager;

LRESULT mt::windows::WM_Activate::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	if (LOWORD(wParam) == WA_INACTIVE)
	{
		_time_manager->Pause();
	}
	else
	{
		_time_manager->Continue();
	}

	return 0;
}

