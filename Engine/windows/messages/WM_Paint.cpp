// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

module WindowsMessages.Paint;

import Engine;

LRESULT mt::windows::WM_Paint::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
