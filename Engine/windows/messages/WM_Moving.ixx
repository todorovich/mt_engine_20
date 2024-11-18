// Copyright 2018 Micho Todorovich, all rights reserved.
export module WindowsMessages.Moving;

export import WindowsMessage;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_Moving : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}