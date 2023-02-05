// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManagerInterface;

export import <memory>;
export import <stdexcept>;

export import WindowsMessage;

export namespace mt::windows
{
	class WindowsMessageManagerInterface
	{
		static std::unique_ptr<WindowsMessageManagerInterface> _defaultWindowsMessageHandler;

		static WindowsMessageManagerInterface* _instance;

	public:
		static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			// Forward hwnd on because we can get messages (e.g., WM_CREATE)
			// before CreateWindow returns, and thus before mhMainWnd is valid.
			return _instance->handle_message(hwnd, msg, wParam, lParam);
		}

		WindowsMessageManagerInterface();

		virtual ~WindowsMessageManagerInterface()
		{
			_instance = _defaultWindowsMessageHandler.get();
		};

		WindowsMessageManagerInterface(const WindowsMessageManagerInterface&) noexcept = default;
		WindowsMessageManagerInterface(WindowsMessageManagerInterface&&) noexcept = default;
		WindowsMessageManagerInterface& operator=(const WindowsMessageManagerInterface&) noexcept = default;
		WindowsMessageManagerInterface& operator=(WindowsMessageManagerInterface&&) noexcept = default;

		virtual LRESULT handle_message(
			const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
		) = 0;
	};

	class NullWindowsMessageManager : public WindowsMessageManagerInterface
	{
	public:
		virtual LRESULT handle_message(
			const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam
		) override
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		};
	};
}