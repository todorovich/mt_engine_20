// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.KeyUp;

import Engine;

LRESULT mt::windows::WM_KeyUp::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_KEY_UP_Handler\n");

	mt::Engine::GetInputManager().KeyUp(wParam, lParam);

	// An application should return zero if it processes this message.
	return 0;
}

