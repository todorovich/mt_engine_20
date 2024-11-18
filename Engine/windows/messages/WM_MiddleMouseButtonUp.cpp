// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.MiddleMouseButtonUp;

import Engine;
import InputModel;
import Windows;

using namespace windows;
using namespace mt::input;
using namespace mt::input::model;

LRESULT mt::windows::WM_MiddleMouseButtonUp::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
) 
{
	_engine->getInputManager()->acceptInput(
		InputType(
			InputDevice::MOUSE, InputDataType::BUTTON_RELEASED,  InputContext::NO_CONTEXT, VirtualKeyCode::THREE
		)
	);

	return 0;
}
