// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module WindowsWindowManager;

export import WindowManagerInterface;
export import WindowsMessageManager;

export import Engine;

export namespace mt::windows
{
	class WindowsWindowManager : public WindowManagerInterface
	{
		std::unique_ptr<WindowsMessageManagerInterface>	_windows_message_manager;

		mt::Engine& _engine;

		HINSTANCE _instance_handle;

		WNDCLASS _window_class;

		HWND _main_window_handle;
	public:

		WindowsWindowManager(mt::Engine& engine) noexcept
			: _windows_message_manager(std::make_unique<WindowsMessageManager>(engine))
			, _engine(engine)
			, _instance_handle(GetModuleHandle(nullptr))
		{

		}

		virtual ~WindowsWindowManager() noexcept
		{
			UnregisterClass(_window_class.lpszClassName, _window_class.hInstance);
		}

		virtual std::expected<void, mt::error::Error> initialize() noexcept override;
		virtual std::expected<void, mt::error::Error> shutdown() noexcept override;

		[[nodiscard]] virtual std::expected<void, mt::error::Error> resize(int width, int height) noexcept override;

		[[nodiscard]] HINSTANCE getInstanceHandle() const noexcept { return _instance_handle; }

		[[nodiscard]] virtual HANDLE getMainWindowHandle() const noexcept override {
			return static_cast<HANDLE>(_main_window_handle);
		}
	};
}