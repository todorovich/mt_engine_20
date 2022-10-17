// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.Activate;

export import WindowsMessage;

export namespace mt::time { class TimeManager; }

export import std.core;

export namespace mt::windows
{
    class WM_Activate : public WindowsMessage
    {
        time::TimeManager* _time_manager;

        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    
    public:

        WM_Activate(time::TimeManager* time_manager)
            : _time_manager(time_manager)
        {};
    };
}