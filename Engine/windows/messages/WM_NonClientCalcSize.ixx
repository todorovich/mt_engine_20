// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessages.NonClientCalcSize;

export import WindowsMessage;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_NonClientCalcSize : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}
