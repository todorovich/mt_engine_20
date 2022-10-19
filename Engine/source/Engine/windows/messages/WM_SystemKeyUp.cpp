
module;

#include <windows.h>

module WindowsMessages.SystemKeyUp;

import Engine;
import InputManager;

LRESULT mt::windows::WM_SystemKeyUp::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	_input_manager->KeyboardEvent(
		static_cast<mt::input::KeyboardKeys>(wParam),
		mt::input::KeyState::RELEASED
	);

	// An application should return zero if it processes this message.
	return 0;
}

