// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.SetText;

export import WindowsMessage;

export namespace mt::windows
{
    class WM_SetText : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}
