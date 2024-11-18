// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsMessage;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class WindowsMessage
    {
    public:
        
        virtual ~WindowsMessage() = default;

        virtual LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam) = 0;
    };
}

module : private;

LRESULT mt::windows::WindowsMessage::execute(
    [[maybe_unused]]const HWND& hwnd, [[maybe_unused]] const UINT& msg, [[maybe_unused]] const WPARAM& wParam, [[maybe_unused]] const LPARAM& lParam
) 
{ 
    return 0; 
};