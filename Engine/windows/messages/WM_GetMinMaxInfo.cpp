// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.GetMinMaxInfo;

import Engine;

import Windows;

using namespace windows;

LRESULT  mt::windows::WM_GetMinMaxInfo::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;

	return 0;
}
