// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.RightMouseButtonDown;

import Engine;
import InputManager;
import InputModel;

LRESULT mt::windows::WM_RightMouseButtonDown::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//MK_SHIFT and MK_CONTROL exist

	_input_manager->acceptInput(
		mt::input::InputType(
			mt::input::InputDevice::MOUSE, mt::input::InputDataType::BUTTON_PRESSED, mt::input::InputContext::NO_CONTEXT, mt::input::VirtualKeyCode::TWO
		)
	);

	return 0;
}
