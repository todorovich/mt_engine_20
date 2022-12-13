// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.MouseMove;

#pragma warning( push )
#pragma warning( disable : 5050 )
import std.core;
#pragma warning( pop )

import Engine;
import InputManager;
import InputModel;

import MicrosoftVirtualKeyCode;

import InputDevice;
import InputDataType;
import InputContext;
import VirtualKeyCode;

LRESULT mt::windows::WM_MouseMove::execute(
	[[maybe_unused]] const HWND & hwnd, [[maybe_unused]] const UINT & msg, [[maybe_unused]] const WPARAM & wParam, const LPARAM & lParam
)
{
	_input_manager->acceptInput(
		mt::input::InputType(mt::input::InputDevice::MOUSE, mt::input::InputDataType::TWO_DIMENSIONAL),
		mt::input::InputData2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))
	);

	return 0; // If an application processes this message, it should return zero.
}
