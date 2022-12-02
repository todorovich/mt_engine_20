// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.MiddleMouseButtonDown;

import Engine;
import InputManager;
import InputModel;

import InputDevice;
import InputDataType;
import InputContext;
import VirtualKeyCode;

LRESULT mt::windows::WM_MiddleMouseButtonDown::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	_input_manager->acceptInput(
		mt::input::InputType(
			mt::input::InputDevice::MOUSE, mt::input::InputDataType::BUTTON_PRESSED, mt::input::InputContext::NO_CONTEXT, mt::input::VirtualKeyCode::THREE
		)
	);

	return 0;
}
