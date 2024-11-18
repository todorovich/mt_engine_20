// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessages.SystemKeyUp;

export import BaseWindowsMessage;

import Engine;
import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_SystemKeyUp : public BaseWindowsMessage
    {
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

    public:

        WM_SystemKeyUp(mt::Engine* engine)
            : BaseWindowsMessage(engine)
        {}
    };
}
