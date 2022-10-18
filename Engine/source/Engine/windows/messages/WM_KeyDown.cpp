// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.KeyDown;

import Engine;
import InputManager;

LRESULT mt::windows::WM_KeyDown::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto key_held_mask = 0x40000000;

	bool repeated = lParam & key_held_mask;

	_input_manager->KeyboardEvent(
		static_cast<mt::input::KeyboardKeys>(wParam),
		repeated ? mt::input::KeyState::HELD : mt::input::KeyState::PRESSED
	);

	// An application should return zero if it processes this message.
	return 0;
}

