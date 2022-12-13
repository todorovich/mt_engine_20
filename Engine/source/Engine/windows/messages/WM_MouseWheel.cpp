// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.MouseWheel;

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

LRESULT mt::windows::WM_MouseWheel::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	// wParam is the WHEEL_DELTA

	_input_manager->acceptInput(
		mt::input::InputType(
			mt::input::InputDevice::MOUSE, mt::input::InputDataType::ONE_DIMENSIONAL, mt::input::InputContext::NO_CONTEXT
		),
		mt::input::InputData1D(static_cast<int>(wParam))
	);

	return 0;
}