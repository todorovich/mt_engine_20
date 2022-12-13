// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.SystemKeyDown;

#pragma warning( push )
#pragma warning( disable : 5050 )
import std.core;
#pragma warning( pop )

import Engine;
import InputManager;
import InputModel;

import InputContext;
import VirtualKeyCode;

LRESULT mt::windows::WM_SystemKeyDown::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
)
{
	auto key_held_mask = 0x40000000;

	bool repeated = lParam & key_held_mask;

	_input_manager->acceptInput(
		mt::input::InputType(
			(repeated ? mt::input::InputDataType::BUTTON_HELD : mt::input::InputDataType::BUTTON_RELEASED),
			mt::input::InputContext::NO_CONTEXT,
			static_cast<mt::input::MicrosoftVirtualKeyCode>(wParam)
		)
	);

	// An application should return zero if it processes this message.
	return 0;
}

