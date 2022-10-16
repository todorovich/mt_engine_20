// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.EraseBackground;

export import WindowsMessage;

export namespace mt::windows
{
    class WM_EraseBackground : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}