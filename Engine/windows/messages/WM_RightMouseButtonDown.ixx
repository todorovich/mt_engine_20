// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.RightMouseButtonDown;

export import WindowsMessage;

export import Engine;

export namespace mt::windows
{
    class WM_RightMouseButtonDown : public WindowsMessage
    {
		mt::Engine* _engine;

        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);

    public:
        WM_RightMouseButtonDown(mt::Engine* engine)
            : _engine(engine)
        {}
    };
}
