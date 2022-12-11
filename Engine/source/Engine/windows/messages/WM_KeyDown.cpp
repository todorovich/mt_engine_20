// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.KeyDown;

import Engine;
import InputManager;
import InputModel;

import InputDataType;
import InputContext;
import MicrosoftVirtualKeyCode;

LRESULT mt::windows::WM_KeyDown::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
)
{
	auto key_held_mask = 0x40000000;

	bool repeated = lParam & key_held_mask;

	_input_manager->acceptInput(
		mt::input::InputType(
			(repeated ? mt::input::InputDataType::BUTTON_HELD : mt::input::InputDataType::BUTTON_PRESSED),
			mt::input::InputContext::NO_CONTEXT,
			static_cast<mt::input::MicrosoftVirtualKeyCode>(wParam)
		)
	);

	// An application should return zero if it processes this message.
	return 0;
}

