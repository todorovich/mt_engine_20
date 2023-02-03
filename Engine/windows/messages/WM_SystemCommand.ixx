// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.SystemCommand;

export import WindowsMessage;

export namespace  mt::input { class InputManagerInterface; }

export namespace mt::windows
{
    class WM_SystemCommand: public WindowsMessage
    {
        LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

    public:

        WM_SystemCommand() = default;
    };
}
