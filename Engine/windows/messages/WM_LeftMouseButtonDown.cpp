// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.LeftMouseButtonDown;

import Engine;

using namespace mt::input;

LRESULT mt::windows::WM_LeftMouseButtonDown::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	_engine->getInputManager()->acceptInput(
		InputType(
			InputDevice::MOUSE, InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, VirtualKeyCode::ONE
		)
	);

	return 0;
}
