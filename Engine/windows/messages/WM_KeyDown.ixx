// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.KeyDown;

export import WindowsMessage;

export namespace  mt::input { class InputManager; }

export namespace mt::windows
{
    class WM_KeyDown : public WindowsMessage
    {
        mt::input::InputManager* _input_manager;

        LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

    public:

        WM_KeyDown(mt::input::InputManager* input_manager)
            : _input_manager(input_manager)
        {}
    };
}
