// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessages.Timer;

export import WindowsMessage;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_Timer : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}
