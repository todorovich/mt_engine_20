// Copyright 2024 Micho Todorovich, all rights reserved.
export module WindowsWindowManager;

export import Engine;
export import MakeUnique;
export import WindowManagerInterface;
export import Window;
export import WindowsMessageManager;

import std;
import Windows;

using namespace windows;
using namespace mt::error;
using namespace mt::windows;
using namespace mt::memory;
using namespace std::literals;

export namespace mt::windows
{
	class WindowsWindowManager : public WindowManagerInterface
	{
		std::unique_ptr<WindowsMessageManagerInterface>	_windows_message_manager;

		mt::Engine& _engine;

		HINSTANCE _instance_handle;

		WNDCLASS _window_class;

		std::unique_ptr<Window> _window;

		bool _registered = false;
	public:
		WindowsWindowManager(mt::Engine& engine, std::error_condition& error, int primary_screen_width, int primary_screen_height) noexcept
			: WindowManagerInterface(primary_screen_width, primary_screen_height)
			, _windows_message_manager(std::make_unique<WindowsMessageManager>(engine, error))
			, _engine(engine)
			, _instance_handle(GetModuleHandle(nullptr))
		{
			if (error.value() != static_cast<int>(ErrorCode::ERROR_UNINITIALIZED)) return;

			_window_class.style = CS_HREDRAW | CS_VREDRAW;
			_window_class.lpfnWndProc = WindowsMessageManagerInterface::MainWndProc;
			_window_class.cbClsExtra = 0;
			_window_class.cbWndExtra = 0;
			_window_class.hInstance = _instance_handle;
			_window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
			_window_class.hCursor = LoadCursor(0, IDC_ARROW);
			_window_class.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
			_window_class.lpszMenuName = 0;
			_window_class.lpszClassName = L"MainWnd";

			if (!RegisterClass(&_window_class))
			{
				Assign(error, ErrorCode::WINDOW_MANAGER_FAILURE);
			}
			else _registered = true;
		}

		virtual ~WindowsWindowManager() noexcept
		{
			if (_registered)
			{
				UnregisterClass(_window_class.lpszClassName, _window_class.hInstance);
				_registered = false;
			}
		}
		
		WindowsWindowManager(const WindowsWindowManager&) noexcept = default;
		WindowsWindowManager(WindowsWindowManager&&) noexcept = default;
		WindowsWindowManager& operator=(const WindowsWindowManager&) noexcept = default;
		WindowsWindowManager& operator=(WindowsWindowManager&&) noexcept = default;

		[[nodiscard]] virtual std::expected<void, std::error_condition> createMainWindow() noexcept override;
		[[nodiscard]] virtual std::expected<void, std::error_condition> destroyMainWindow() noexcept override;
		[[nodiscard]] virtual std::expected<void, std::error_condition> runMessageLoop() noexcept override;
		[[nodiscard]] virtual bool isMessageLoopRunning() noexcept override;

		[[nodiscard]] virtual std::expected<void, std::error_condition> resize(int width, int height) noexcept override;

		[[nodiscard]] HINSTANCE getInstanceHandle() const noexcept { return _instance_handle; }

		[[nodiscard]] virtual Window* getWindow() const noexcept override { return _window.get(); }

		virtual void toggleShowCursor() noexcept override;
	};
}