// Copyright 2022 Micho Todorovich, all rights reserved.
module;

export module WindowsMessages.GetIcon;

export import WindowsMessage;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_GetIcon : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}