// Copyright 2024 Micho Todorovich, all rights reserved.
module WindowsMessages.Quit;

import Engine;
import Windows;

using namespace windows;

LRESULT mt::windows::WM_Quit::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam)
{
	// This should be the final windows message received as this causes the windows message loop to stop
	// and the main engine tick thread to be joined

	// Command doesn't actually ever get called.

	//OutputDebugStringW(L"WM_Quit\n");

	return 0;
}
