// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.RightMouseButtonDown;

import Engine;
import InputModel;
import Windows;

using namespace windows;
using namespace mt::input::model;

LRESULT mt::windows::WM_RightMouseButtonDown::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	//MK_SHIFT and MK_CONTROL exist

	_engine->getInputManager()->acceptInput(
		InputType(
			InputDevice::MOUSE, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::TWO
		)
	);

	return 0;
}
