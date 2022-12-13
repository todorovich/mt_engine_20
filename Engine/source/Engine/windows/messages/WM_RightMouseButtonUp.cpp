// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.RightMouseButtonUp;

#pragma warning( push )
#pragma warning( disable : 5050 )
import std.core;
#pragma warning( pop )

import Engine;
import InputManager;
import InputModel;

import InputDevice;
import InputDataType;
import InputContext;
import VirtualKeyCode;

LRESULT mt::windows::WM_RightMouseButtonUp::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	_input_manager->acceptInput(
		mt::input::InputType(
			mt::input::InputDevice::MOUSE, mt::input::InputDataType::BUTTON_RELEASED, mt::input::InputContext::NO_CONTEXT, mt::input::VirtualKeyCode::TWO
		)
	);

	return 0;
}
