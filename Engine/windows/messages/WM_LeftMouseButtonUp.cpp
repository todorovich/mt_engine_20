// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>

module WindowsMessages.LeftMouseButtonUp;

import Engine;

using namespace mt::windows;

LRESULT mt::windows::WM_LeftMouseButtonUp::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	_engine->getInputManager()->acceptInput(
		mt::input::InputType(
			mt::input::InputDevice::MOUSE, mt::input::InputDataType::BUTTON_RELEASED, mt::input::InputContext::NO_CONTEXT, mt::input::VirtualKeyCode::ONE
		)
	);

	return 0;
}
