// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.SystemKeyUp;

import Engine;
import InputModel;
import Windows;

using namespace windows;

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

