// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessages.LeftMouseButtonDown;

export import BaseWindowsMessage;

import Engine;
import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_LeftMouseButtonDown : public BaseWindowsMessage
    {
		LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);

    public:
        WM_LeftMouseButtonDown(mt::Engine* engine)
            : BaseWindowsMessage(engine)
        {}

    };
}
