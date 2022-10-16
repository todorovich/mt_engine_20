// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.Input;

export import WindowsMessage;

export namespace mt::windows
{
    class WM_Input : public WindowsMessage
    {
        unsigned buffer_size = 40;

        char *buffer = nullptr;

        UINT cbSizeT = 64;

        PRAWINPUT pRawInput = nullptr;

    public:
        WM_Input();

        ~WM_Input();

        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}
