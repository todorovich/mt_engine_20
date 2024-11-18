// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.SystemKeyDown;

import Engine;
import InputModel;
import Windows;

using namespace windows;
using namespace mt::input::model;

LRESULT mt::windows::WM_SystemKeyDown::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
)
{
	auto key_held_mask = 0x40000000;

	bool repeated = lParam & key_held_mask;

	_engine->getInputManager()->acceptInput(
		InputType(
			(repeated ? InputDataType::BUTTON_HELD : InputDataType::BUTTON_RELEASED),
			InputContext::NO_CONTEXT,
			static_cast<MicrosoftVirtualKeyCode>(wParam)
		)
	);

	// An application should return zero if it processes this message.
	return 0;
}

