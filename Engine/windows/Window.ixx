module;

#include <windows.h>

export module Window;

export import <expected>;
export import <memory>;

export import Error;

using namespace mt::error;
using namespace std::literals;

export namespace mt::windows
{
	class Window
	{
		const std::wstring _main_window_caption = L"mt_engine";
		// TODO UNION this with the engine handle. or at least verify in testing they are converible.
		HANDLE _main_window_handle = nullptr;
	public:

		Window(Error& error, int width, int height, HINSTANCE instance_handle)
		{
			// Compute window rectangle dimensions based on requested client area dimensions.
			RECT rectangle = { 0, 0, width, height };

			// TODO: this gives you the actual size of your window including the non client area (border and menus)
			// 	not valid with borderless fullscreen will be necessary when I implement non-fullscreen.
			//AdjustWindowRect(&rectangle, WS_OVERLAPPEDWINDOW, false);

			int actual_width = rectangle.right - rectangle.left;
			int actual_height = rectangle.bottom - rectangle.top;

			// Creating a window causes the message loop to start, which will call the renderer to do the initial resize.
			// as we are constructed before the renderer this must be called outside the constructor.
			_main_window_handle = CreateWindowW(
				L"MainWnd", _main_window_caption.c_str(), WS_MAXIMIZE, 0, 0,
				actual_width, actual_height, nullptr, nullptr, instance_handle, 0
			);

			if (!_main_window_handle)
			{
				error = Error{
					L"Unable to create the window."sv,
					mt::error::ErrorCode::WINDOW_MANAGER_FAILURE,
					__func__, __FILE__, __LINE__
				};
			}

			SetWindowLongPtrW(static_cast<HWND>(_main_window_handle), GWL_STYLE, 0); //remove all window styles, check MSDN for details

			ShowWindow(static_cast<HWND>(_main_window_handle), SW_SHOWMAXIMIZED); //display window

		}

		[[nodiscard]] HANDLE getHandle() const noexcept {
			return _main_window_handle;
		}

		~Window()
		{
			if (_main_window_handle != nullptr)
				destroyWindow();
		}

		void destroyWindow()
		{
			DestroyWindow(static_cast<HWND>(_main_window_handle));
		}
	};
}