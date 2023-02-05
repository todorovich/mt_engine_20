// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.XButtonUp;

import InputModel;

using namespace mt::input::model;

LRESULT mt::windows::WM_XButtonUp::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	bool button_1 = 0x00010000 & wParam;
	bool button_2 = 0x00020000 & wParam;

	if (button_1)
	{
		_engine->getInputManager()->acceptInput(
			InputType(
				InputDevice::MOUSE, InputDataType::BUTTON_RELEASED, InputContext::NO_CONTEXT, VirtualKeyCode::FOUR
			)
		);

		return true; // return true if processed.
	}
	else if (button_2)
	{
		_engine->getInputManager()->acceptInput(
			InputType(
				InputDevice::MOUSE, InputDataType::BUTTON_RELEASED, InputContext::NO_CONTEXT, VirtualKeyCode::FIVE
			)
		);

		return true; // return true if processed.
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}