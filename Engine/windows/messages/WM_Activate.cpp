// Copyright 2022 Micho Todorovich, all rights reserved.
module WindowsMessages.Activate;

import Windows;

using namespace windows;

using namespace mt::input::model;

LRESULT mt::windows::WM_Activate::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	if (LOWORD(wParam) == WA_INACTIVE)
	{
		// TODO: give specific method to notify input manager that we lost focus.
		_engine->getInputManager()->acceptInput(InputType());

		_engine->getTimeManager()->pause();
	}
	else
	{
		_engine->getTimeManager()->resume();
	}

	return 0;
}

