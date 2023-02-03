// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.KeyUp;

import InputModel;

using namespace mt::input::model;

LRESULT mt::windows::WM_KeyUp::execute([[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam)
{
	_engine->getInputManager()->acceptInput(
		InputType(
			InputDataType::BUTTON_RELEASED, InputContext::NO_CONTEXT, static_cast<MicrosoftVirtualKeyCode>(wParam)
		)
	);

	// An application should return zero if it processes this message.
	return 0;
}

