// Copyright 2024 Micho Todorovich, all rights reserved.
module;

#include <cstdint>

module WindowsMessages.SystemCommand;

import Engine;
import Windows;

using namespace windows;

LRESULT mt::windows::WM_SystemCommand::execute(
	[[maybe_unused]] const HWND& hwnd, [[maybe_unused]] const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	if (wParam == SC_KEYMENU)
	{
		OutputDebugString((std::format(L"{}", static_cast<int64_t>(lParam))).c_str());
	}

	// An application should return zero if it processes this message.
	return 0;
}

