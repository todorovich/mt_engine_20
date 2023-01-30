// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.KeyDown;

export import WindowsMessage;

export import Engine;

export namespace mt::windows
{
    class WM_KeyDown : public WindowsMessage
    {
		mt::Engine* _engine;

        LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

    public:

        WM_KeyDown(mt::Engine* engine)
            : _engine(engine)
        {}
    };
}
