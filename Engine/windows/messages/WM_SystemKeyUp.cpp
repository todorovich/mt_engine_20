
module;

#include <windows.h>

module WindowsMessages.SystemKeyUp;

import Engine;
import InputModel;

using namespace mt::input::model;

LRESULT mt::windows::WM_SystemKeyUp::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	_engine->getInputManager()->acceptInput(
		InputType(InputDataType::BUTTON_PRESSED, InputContext::NO_CONTEXT, static_cast<MicrosoftVirtualKeyCode>(wParam))
	);

	// An application should return zero if it processes this message.
	return 0;
}

