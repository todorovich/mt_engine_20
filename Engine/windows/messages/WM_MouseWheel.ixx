// Copyright 2022 Micho Todorovich, all rights reserved.
export module WindowsMessages.MouseWheel;

export import BaseWindowsMessage;

import Engine;
import Windows;

using namespace windows;

export namespace mt::windows
{
    class WM_MouseWheel : public BaseWindowsMessage
    {
		mt::Engine* _engine;

        LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

    public:

        WM_MouseWheel(mt::Engine* engine)
            : BaseWindowsMessage(engine)
        {}
    };
}
