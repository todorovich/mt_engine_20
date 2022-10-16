// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module Engine;

//import Archiver;
export import CommandManager;
export import LogManager;
export import TimeManager;
export import Status;
export import WindowsMessageManager;
export import InputManager;
export import DirectXRenderer;
export import Camera;

export import std.core;

export using namespace std::literals::chrono_literals;

export LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

export namespace mt
{
	class Engine
	{
	 private:
		//io::Archiver archiver;

		renderer::DirectXRenderer direct_x_renderer_;
		windows::WindowsMessageManager windows_message_manager_;
		command::CommandManager command_manager_;
		input::InputManager input_manager_;
		time::TimeManager time_manager_;
		logging::LogManager log_manager_;

		std::wstring _main_window_caption = L"mt_engine";
		std::string _main_window_caption_string = "mt_engine";

		time::Duration _time_since_stat_update = time::Duration(0);

		HINSTANCE _instance_handle = nullptr;// application instance handle
		HWND _main_window_handle = nullptr;  // main window handle

		int _window_width = 0;
		int _window_height = 0;
		float _window_aspect_ratio = _window_height ? static_cast<float>(_window_width) / _window_height : 0;

		volatile bool _is_window_minimized = false; // is the application minimized?
		volatile bool _is_window_maximized = false; // is the application maximized?
		volatile bool _is_window_resizing = false;  // are the Resize bars being dragged?
		volatile bool _is_window_fullscreen = false;// fullscreen enabled
		volatile bool _is_shutting_down = false;    // Shutdown is checked to see if Tick should keep ticking, on true ticking stops and Tick() returns
		bool _was_rendered_this_frame = false;

	 protected:
		static std::unique_ptr<Engine> _instance;

		std::thread _engine_tick_thread;

		// CALLBACKS
		virtual void _OnResize(){};
		virtual void _Update(){};
		virtual void _Draw(){};

		// ACCESSORS
		HINSTANCE _GetInstanceHandle() const
		{
			return _instance_handle;
		};

		HWND _GetMainWindowHandle() const
		{
			return _main_window_handle;
		};

		float _GetWindowAspectRatio() const
		{
			return _window_aspect_ratio;
		};

		int _GetWindowWidth() const
		{
			return _window_width;
		};

		int _GetWindowHeight() const
		{
			return _window_height;
		};

		bool _GetIsWindowMaximized() const
		{
			return _is_window_maximized;
		};

		bool _GetIsWindowMinimized() const
		{
			return _is_window_minimized;
		};

		bool _GetIsWindowResizing() const
		{
			return _is_window_resizing;
		};

		bool _GetIsWindowFullscreen() const
		{
			return _is_window_fullscreen;
		};

		virtual bool _Initialize(HINSTANCE hInstance);
		Status _Run();
		void _Tick();

		bool _InitializeMainWindow();
		void _UpdateFrameStatisticsNoTimeCheck(bool was_rendered);

		void _resize(int width, int height);

		void _set_is_resizing(bool is_resizing)
		{
			_is_window_resizing = is_resizing;
		}

		void _set_is_maximized(bool is_maximized)
		{
			_is_window_maximized = is_maximized;
		}

		void _set_is_minimized(bool is_minimized)
		{
			_is_window_minimized = is_minimized;
		}

		void _shutdown()
		{
			time_manager_.Pause();

			_is_shutting_down = true;

			// Destroy the window
			DestroyWindow(GetEngine()._GetMainWindowHandle());
		}

		void _destroy()
		{
			delete _instance.release();
		}

	 public:
		// ACCESSOR

		static Engine& GetEngine()
		{
			return *Engine::_instance.get();
		};
		static time::TimeManager& GetTimeManager()
		{
			return GetEngine().time_manager_;
		};
		static renderer::DirectXRenderer& GetRenderer()
		{
			return GetEngine().direct_x_renderer_;
		};
		static input::InputManager& GetInputManager()
		{
			return GetEngine().input_manager_;
		}
		static camera::Camera& GetCurrentCamera()
		{
			return GetEngine().direct_x_renderer_.GetCurrentCamera();
		}
		static windows::WindowsMessageManager& GetWindowsMessageManager()
		{
			return GetEngine().windows_message_manager_;
		};
		static command::CommandManager& GetCommandManager()
		{
			return GetEngine().command_manager_;
		}
		static logging::LogManager& GetLogManager()
		{
			return GetEngine().log_manager_;
		}

		static HINSTANCE GetInstanceHandle()
		{
			return GetEngine()._GetInstanceHandle();
		};
		static HWND GetMainWindowHandle()
		{
			return GetEngine()._GetMainWindowHandle();
		};

		static float GetWindowAspectRatio()
		{
			return GetEngine()._GetWindowAspectRatio();
		};
		static int GetWindowWidth()
		{
			return GetEngine()._GetWindowWidth();
		};
		static int GetWindowHeight()
		{
			return GetEngine()._GetWindowHeight();
		};

		static bool IsWindowMaximized()
		{
			return GetEngine()._GetIsWindowMaximized();
		};
		static bool IsWindowMinimized()
		{
			return GetEngine()._GetIsWindowMinimized();
		};
		static bool IsWindowResizing()
		{
			return GetEngine()._GetIsWindowResizing();
		};
		static bool IsWindowFullscreen()
		{
			return GetEngine()._GetIsWindowFullscreen();
		};

		static bool IsDestroyed()
		{
			return _instance.get() == nullptr;
		};

		// MUTATOR

		static Status Run()
		{
			return (_instance.get() == nullptr) ? Status::failure : GetEngine()._Run();
		};

		static bool Initialize(HINSTANCE hInstance)
		{
			return GetEngine()._Initialize(hInstance);
			;
		};

		static void Resize(int width, int height)
		{
			return GetEngine()._resize(width, height);
		};

		static void SetIsWindowResizing(bool is_resizing)
		{
			return GetEngine()._set_is_resizing(is_resizing);
		}

		static void SetIsWindowMaximized(bool is_maximized)
		{
			return GetEngine()._set_is_maximized(is_maximized);
		}

		static void SetIsWindowMinimized(bool is_resizing)
		{
			return GetEngine()._set_is_minimized(is_resizing);
		}

		static void SetWindowDimensions(const int& width, const int& height)
		{
			GetEngine()._window_width = width;
			GetEngine()._window_height = height;
			GetEngine()._window_aspect_ratio = (float)width / (float)height;
		}

		// Called to begin orderly shutdown.
		static void Shutdown()
		{
			GetEngine()._shutdown();
			OutputDebugStringW(L"Engine Shutdown Initiated\n");
		}

		static void Destroy()
		{
			if (_instance.get() != nullptr)
			{
				GetEngine()._destroy();
			}

			OutputDebugStringW(L"Engine Destroyed\n");
		}

		Engine() = default;

		virtual ~Engine()
		{
			if (!IsDestroyed())
			{
				Destroy();
			}
		};

		Engine(const Engine& other) = delete;

		Engine(Engine&& other) = delete;

		Engine& operator=(const Engine& other) = delete;

		Engine& operator=(Engine&& other) = delete;
	};
}

