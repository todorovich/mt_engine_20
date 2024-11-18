// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessages.KeyUp;

export import BaseWindowsMessage;

import Engine;
import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_KeyUp : public BaseWindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);

    public:

        WM_KeyUp(mt::Engine* engine)
            : BaseWindowsMessage(engine)
        {}
    };
}
