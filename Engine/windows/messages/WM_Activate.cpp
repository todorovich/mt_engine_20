// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Activate;

LRESULT mt::windows::WM_Activate::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	if (LOWORD(wParam) == WA_INACTIVE)
	{
		// TODO: give specific method to notify input manager that we lost focus.
		_engine->getInputManager()->acceptInput(mt::input::InputType());

		_engine->getTimeManager()->pause();
	}
	else
	{
		_engine->getTimeManager()->resume();
	}

	return 0;
}

