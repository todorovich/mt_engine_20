// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessages.RightMouseButtonDown;

export import BaseWindowsMessage;

import Engine;
import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_RightMouseButtonDown : public BaseWindowsMessage
    {
		LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);

    public:
        WM_RightMouseButtonDown(mt::Engine* engine)
            : BaseWindowsMessage(engine)
        {}
    };
}
