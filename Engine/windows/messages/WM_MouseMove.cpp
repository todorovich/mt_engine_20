// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.MouseMove;

import Engine;
import InputModel;

using namespace mt::input::model;

LRESULT mt::windows::WM_MouseMove::execute(
	[[maybe_unused]] const HWND & hwnd, [[maybe_unused]] const UINT & msg, [[maybe_unused]] const WPARAM & wParam, const LPARAM & lParam
)
{
	_engine->getInputManager()->acceptInput(
		InputType(InputDevice::MOUSE, InputDataType::TWO_DIMENSIONAL),
		InputData2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))
	);

	return 0; // If an application processes this message, it should return zero.
}
