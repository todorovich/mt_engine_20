module;

#include <windows.h>

export module WindowsWindowManager;

export import WindowManagerInterface;

export import Engine;

export namespace mt::windows
{
	class WindowsWindowManager : public WindowManagerInterface
	{
		mt::Engine* _engine;

		HINSTANCE _instance_handle;

		WNDCLASS _window_class;

		HWND _main_window_handle;
	public:

		WindowsWindowManager(mt::Engine* engine, HINSTANCE instance_handle) noexcept
			: _engine(engine)
			, _instance_handle(instance_handle)
		{}

		virtual ~WindowsWindowManager() noexcept
		{
			UnregisterClass(_window_class.lpszClassName, _window_class.hInstance);
		}

		virtual std::expected<void, mt::Error> initialize() noexcept override;
		virtual std::expected<void, mt::Error> shutdown() noexcept override;

		[[nodiscard]] virtual std::expected<void, mt::Error> resize(int width, int height) noexcept override;

		[[nodiscard]] HINSTANCE getInstanceHandle() const noexcept { return _instance_handle; }

		[[nodiscard]] virtual HANDLE getMainWindowHandle() const noexcept override {
			return static_cast<HANDLE>(_main_window_handle);
		}
	};
}