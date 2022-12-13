// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.destroy;

#pragma warning( push )
#pragma warning( disable : 5050 )
import std.core;
#pragma warning( pop )

import Engine;

// This message is sent when a window is being destroyed. It is sent to the window procedure of 
// the window being destroyed after the window is removed from the screen.
// This message is sent first to the window being destroyed and then to the child windows, if any, 
// as they are destroyed.During the processing of the message, it can be assumed that all child windows still exist.
LRESULT mt::windows::WM_Destroy::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
)
{
	OutputDebugStringW(L"WM_Destroy\n");

	// Causes quit message to appear which will end the windows message processing thread.
	PostQuitMessage(0);
	
	return 0;
}
