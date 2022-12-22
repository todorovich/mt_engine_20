// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.Quit;

export import WindowsMessage;

export namespace mt::windows
{
    // This quit does not give a fuck. Must quit now.
    class WM_Quit : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}
