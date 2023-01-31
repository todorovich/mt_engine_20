// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManagerInterface;

export import WindowsMessage;

export namespace mt::windows
{
	class WindowsMessageManagerInterface
	{
	public:

		WindowsMessageManagerInterface() = default;
		virtual ~WindowsMessageManagerInterface() = default;
		WindowsMessageManagerInterface(const WindowsMessageManagerInterface&) noexcept = default;
		WindowsMessageManagerInterface(WindowsMessageManagerInterface&&) noexcept = default;
		WindowsMessageManagerInterface& operator=(const WindowsMessageManagerInterface&) noexcept = default;
		WindowsMessageManagerInterface& operator=(WindowsMessageManagerInterface&&) noexcept = default;

		virtual LRESULT handle_message(
			const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
		) = 0;
	};
}