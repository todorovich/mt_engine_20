// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.EnterSizeMove;

export import WindowsMessage;

export namespace mt { class Engine; }

export namespace mt::windows
{
    class WM_EnterSizeMove : public WindowsMessage
    {
        mt::Engine& _engine;

        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);

    public:
        WM_EnterSizeMove(mt::Engine& engine)
            : _engine(engine)
        {}
    };
}
