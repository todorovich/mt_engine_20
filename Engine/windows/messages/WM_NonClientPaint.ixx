// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessages.NonClientPaint;

export import WindowsMessage;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_NonClientPaint : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}