// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowManagerInterface;

export import <string>;

export import Error;
export import Window;

using mt::error::Error;

using namespace mt::windows;

export namespace mt::windows 
{
	class WindowManagerInterface
	{
		volatile bool _is_window_minimized = false; // is the application minimized?
		volatile bool _is_window_maximized = false; // is the application maximized?
		volatile bool _is_window_resizing = false;  // are the Resize bars being dragged?
		volatile bool _is_window_fullscreen = false;// fullscreen enabled

		int _window_width = 0;
		int _window_height = 0;

		float _window_aspect_ratio = 0.0f;

	protected:

		void _setWindowAspectRatio(float aspect_ratio) noexcept
		{
			_window_aspect_ratio = aspect_ratio;
		}

		void _setWindowWidth(int width) noexcept
		{
			_window_width = width;
			_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;
		}

		void _setWindowHeight(int height) noexcept
		{
			_window_height = height;
			_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;
		}

	public:
		// TODO:
		using HANDLE = void*;

		WindowManagerInterface() noexcept = default;
		virtual ~WindowManagerInterface() noexcept = default;

		[[nodiscard]] virtual std::expected<void, Error> createMainWindow() noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> destroyMainWindow() noexcept = 0;

		[[nodiscard]] virtual Window* getWindow() const noexcept = 0;

		[[nodiscard]] virtual std::expected<void, Error> resize(int width, int height) noexcept
		{
			_window_width = width;
			_window_height = height;
			_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;
			return {};
		};

		void setIsWindowResizing(bool is_resizing) noexcept { _is_window_resizing = is_resizing; }

		void setIsWindowMaximized(bool is_maximized) noexcept { _is_window_maximized = is_maximized; }

		void setIsWindowMinimized(bool is_minimized) noexcept { _is_window_minimized = is_minimized; }

		bool isWindowMaximized() const noexcept { return _is_window_maximized; };

		bool isWindowMinimized() const noexcept { return _is_window_minimized; };

		bool isWindowResizing() const noexcept { return _is_window_resizing; };

		bool isWindowFullscreen() const noexcept { return _is_window_fullscreen; };

		float getWindowAspectRatio() const noexcept { return _window_aspect_ratio; }

		int getWindowWidth() const noexcept { return _window_width; }

		int getWindowHeight() const noexcept { return _window_height; }
	};
}