
module;

#include <windows.h>

module WindowsMessages.SystemKeyUp;

import Engine;

import InputManager;
import InputModel;

import InputDataType;
import InputContext;

LRESULT mt::windows::WM_SystemKeyUp::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	_input_manager->acceptInput(
		mt::input::InputType(mt::input::InputDataType::BUTTON_PRESSED, mt::input::InputContext::NO_CONTEXT, static_cast<mt::input::MicrosoftVirtualKeyCode>(wParam))
	);

	// An application should return zero if it processes this message.
	return 0;
}

