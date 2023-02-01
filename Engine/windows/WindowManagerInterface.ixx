// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowManagerInterface;

export import <string>;

export import Error;

using mt::error::Error;

export namespace mt::windows 
{
	class WindowManagerInterface
	{
		volatile bool _is_window_minimized = false; // is the application minimized?
		volatile bool _is_window_maximized = false; // is the application maximized?
		volatile bool _is_window_resizing = false;  // are the Resize bars being dragged?
		volatile bool _is_window_fullscreen = false;// fullscreen enabled

	protected:
		const std::wstring _main_window_caption = L"mt_engine";

	public:
		using HANDLE = void*;

		WindowManagerInterface() noexcept = default;
		virtual ~WindowManagerInterface() noexcept = default;

		virtual std::expected<void, Error> initialize() noexcept = 0;

		virtual std::expected<void, Error> shutdown() noexcept = 0;

		[[nodiscard]] virtual HANDLE getMainWindowHandle() const noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> resize(int width, int height) noexcept = 0;

		void setIsWindowResizing(bool is_resizing) noexcept { _is_window_resizing = is_resizing; }

		void setIsWindowMaximized(bool is_maximized) noexcept { _is_window_maximized = is_maximized; }

		void setIsWindowMinimized(bool is_minimized) noexcept { _is_window_minimized = is_minimized; }

		bool isWindowMaximized() const noexcept { return _is_window_maximized; };

		bool isWindowMinimized() const noexcept { return _is_window_minimized; };

		bool isWindowResizing() const noexcept { return _is_window_resizing; };

		bool isWindowFullscreen() const noexcept { return _is_window_fullscreen; };
	};
}