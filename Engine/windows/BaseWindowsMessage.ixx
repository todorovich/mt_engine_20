// Copyright 2024 Micho Todorovich, all rights reserved.
export module BaseWindowsMessage;

export import WindowsMessage;
export import Engine;

import Windows;

using namespace windows;

export namespace mt::windows
{
    class BaseWindowsMessage : public WindowsMessage
    {
    protected:
        mt::Engine* _engine;

        BaseWindowsMessage(mt::Engine* engine)
            : _engine(engine)
        {
        }

    public:

        virtual ~BaseWindowsMessage() = default;

        virtual LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam) = 0;
    };
}