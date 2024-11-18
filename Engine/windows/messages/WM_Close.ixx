// Copyright 2022 Micho Todorovich, all rights reserved.
export module WindowsMessages.Close;

export import BaseWindowsMessage;

import Engine;
import Windows;

using namespace windows;

export namespace mt::windows
{
    // Orderly Shutdown
    class WM_Close : public BaseWindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    
    public:

        WM_Close(mt::Engine* engine)
            : BaseWindowsMessage(engine)
        {}
    };
}