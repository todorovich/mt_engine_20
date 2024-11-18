// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.MenuChar;

import Engine;
import Windows;

using namespace windows;

LRESULT mt::windows::WM_MenuChar::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam)
{
	return MAKELRESULT(0, MNC_CLOSE);
}
