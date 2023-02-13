// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsMessageManagerInterface;

export import <memory>;
export import <stdexcept>;

export import Error;
export import Task;
export import WindowsMessage;

using namespace mt::error;

export namespace mt::windows
{
	class WindowsMessageManagerInterface
	{
		static std::unique_ptr<WindowsMessageManagerInterface> _defaultWindowsMessageHandler;

		static WindowsMessageManagerInterface* _instance;

	protected:
		// For use by the null message manager. Should not be used by any other message manager.
		WindowsMessageManagerInterface() noexcept = default;
	public:
		static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			// Forward hwnd on because we can get messages (e.g., WM_CREATE)
			// before CreateWindow returns, and thus before mhMainWnd is valid.
			return _instance->handle_message(hwnd, msg, wParam, lParam);
		}

		WindowsMessageManagerInterface(Error& error) noexcept;

		virtual ~WindowsMessageManagerInterface() noexcept
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

		virtual void destroyMainWindow() = 0;

		virtual bool hasReceivedQuit() = 0;

		virtual mt::task::Task* getMessageLoopTask() noexcept = 0;
		virtual void toggleShowCursor() noexcept = 0;
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

		virtual void destroyMainWindow() {}

		virtual bool hasReceivedQuit() { return false; }

		virtual mt::task::Task* getMessageLoopTask() noexcept { return nullptr; };

		virtual void toggleShowCursor() noexcept override {};
	};
}