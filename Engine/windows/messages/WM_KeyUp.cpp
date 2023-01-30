// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.KeyUp;

LRESULT mt::windows::WM_KeyUp::execute([[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam)
{
	_engine->getInputManager()->acceptInput(
		mt::input::InputType(
			mt::input::InputDataType::BUTTON_RELEASED, mt::input::InputContext::NO_CONTEXT, static_cast<mt::input::MicrosoftVirtualKeyCode>(wParam)
		)
	);

	// An application should return zero if it processes this message.
	return 0;
}

