// Copyright 2022 Micho Todorovich, all rights reserved.
export module WindowsMessages.destroy;

export import WindowsMessage;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_Destroy : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}