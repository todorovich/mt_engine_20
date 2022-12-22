// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessages.MiddleMouseButtonUp;

export import WindowsMessage;

export namespace mt::input { class InputManager; }

export namespace mt::windows
{
    class WM_MiddleMouseButtonUp : public WindowsMessage
    {
        input::InputManager* _input_manager;

        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);

    public:
        WM_MiddleMouseButtonUp(input::InputManager* input_manager)
            : _input_manager(input_manager)
        {
        }
    };
}