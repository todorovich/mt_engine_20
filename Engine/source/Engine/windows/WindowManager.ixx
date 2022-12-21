module;

#include <windows.h>

export module WindowManager;

export namespace mt { class Engine; }

export import <string>;

export namespace mt::windows 
{
	class WindowManager 
	{
		volatile bool _is_window_minimized = false; // is the application minimized?
		volatile bool _is_window_maximized = false; // is the application maximized?
		volatile bool _is_window_resizing = false;  // are the Resize bars being dragged?
		volatile bool _is_window_fullscreen = false;// fullscreen enabled
	
		HWND _main_window_handle = nullptr;  // main window handle

		HINSTANCE _instance_handle = nullptr;// application instance handle

		mt::Engine& _engine;

		std::wstring _main_window_caption = L"mt_engine";
		std::string _main_window_caption_string = "mt_engine";

	public:

		WindowManager(mt::Engine& engine)
			: _engine(engine)
		{}

		HINSTANCE getInstanceHandle() const { return _instance_handle; }

		HWND getMainWindowHandle() { return _main_window_handle; }

		bool initializeMainWindow(HINSTANCE instance_handle);

		void resize(int width, int height);

		void getIsWindowResizing(bool is_resizing) { _is_window_resizing = is_resizing; }

		void getIsWindowMaximized(bool is_maximized) { _is_window_maximized = is_maximized; }

		void getIsWindowMinimized(bool is_minimized) { _is_window_minimized = is_minimized; }

		void setIsWindowResizing(bool is_resizing) { _is_window_resizing = is_resizing; }

		void setIsWindowMaximized(bool is_maximized) { _is_window_maximized = is_maximized; }

		void setIsWindowMinimized(bool is_minimized) { _is_window_minimized = is_minimized; }

		HWND getMainWindowHandle() const { return _main_window_handle; }

		bool isWindowMaximized() const { return _is_window_maximized; };

		bool isWindowMinimized() const { return _is_window_minimized; };

		bool isWindowResizing() const { return _is_window_resizing; };

		bool isWindowFullscreen() const { return _is_window_fullscreen; };
	};
}