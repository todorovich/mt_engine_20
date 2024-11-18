// Copyright 2022 Micho Todorovich, all rights reserved.
export module WindowsMessages.Activate;

export import BaseWindowsMessage;

import Windows;
import Engine;

using namespace windows;

export namespace mt::windows
{
    class WM_Activate : public BaseWindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    
    public:

        WM_Activate(mt::Engine* engine)
            : BaseWindowsMessage{ engine }
        {}
    };
}