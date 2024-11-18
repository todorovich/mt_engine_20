// Copyright 2022 Micho Todorovich, all rights reserved.
export module WindowsMessages.RightMouseButtonUp;

export import BaseWindowsMessage;

import Engine;
import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_RightMouseButtonUp : public BaseWindowsMessage
    {
		LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);

    public:

        WM_RightMouseButtonUp(mt::Engine* engine)
            : BaseWindowsMessage(engine)
        {}
    };
}
