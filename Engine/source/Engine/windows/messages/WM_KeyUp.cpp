// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.KeyUp;

import Engine;
import InputManager;

LRESULT mt::windows::WM_KeyUp::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_KEY_UP_Handler\n");

	_input_manager->keyboardEvent(
		static_cast<mt::input::KeyboardKeys>(wParam),
		mt::input::KeyState::RELEASED
	);

	// An application should return zero if it processes this message.
	return 0;
}

