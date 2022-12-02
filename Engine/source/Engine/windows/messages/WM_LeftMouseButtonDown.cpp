// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.LeftMouseButtonDown;

import Engine;
import InputManager;
import InputModel;

import InputDevice;
import InputDataType;
import InputContext;
import VirtualKeyCode;

LRESULT mt::windows::WM_LeftMouseButtonDown::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	_input_manager->acceptInput(
		mt::input::InputType(
			mt::input::InputDevice::MOUSE, mt::input::InputDataType::BUTTON_PRESSED, mt::input::InputContext::NO_CONTEXT, mt::input::VirtualKeyCode::ONE
		)
	);

	return 0;
}
