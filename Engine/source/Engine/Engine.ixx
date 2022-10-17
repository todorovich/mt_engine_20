// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module Engine;

export import Time;
export import Status;
export import std.core;

export using namespace std::literals::chrono_literals;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

export namespace mt
{
	namespace renderer { class DirectXRenderer; };
	namespace windows { class WindowsMessageManager; };
	namespace command { class CommandManager; };
	namespace input { class InputManager; };
	namespace time { class TimeManager; };
	namespace logging { class LogManager; };
	namespace camera { class Camera; } // ResearchMe: Should provide access here?

	class Engine
	{
		const std::unique_ptr<command::CommandManager>			command_manager_;
		const std::unique_ptr<logging::LogManager>				log_manager_;
		const std::unique_ptr<input::InputManager>				input_manager_;
		const std::unique_ptr<time::TimeManager>				time_manager_;
		const std::unique_ptr<renderer::DirectXRenderer>		direct_x_renderer_;
		const std::unique_ptr<windows::WindowsMessageManager>	windows_message_manager_;

		std::wstring _main_window_caption = L"mt_engine";
		std::string _main_window_caption_string = "mt_engine";

		time::Duration _time_since_stat_update = time::Duration(0);

		HINSTANCE _instance_handle = nullptr;// application instance handle
		HWND _main_window_handle = nullptr;  // main window handle

		volatile bool _is_window_minimized = false; // is the application minimized?
		volatile bool _is_window_maximized = false; // is the application maximized?
		volatile bool _is_window_resizing = false;  // are the Resize bars being dragged?
		volatile bool _is_window_fullscreen = false;// fullscreen enabled
		volatile bool _is_shutting_down = false;    // Shutdown is checked to see if Tick should keep ticking, on true ticking stops and Tick() returns
		bool _was_rendered_this_frame = false;

	protected:	
		static Engine* _instance;

		std::thread _engine_tick_thread;

		// CALLBACKS
		virtual void _OnResize() {};
		virtual void _Update() {};
		virtual void _Draw() {};

		void _Tick();
		void _UpdateFrameStatisticsNoTimeCheck(bool was_rendered);
		bool _InitializeMainWindow();

	public:
		
		// Big 5
		Engine();

		virtual ~Engine();

		Engine(const Engine& other) = delete;

		Engine(Engine&& other) = delete;

		Engine& operator=(const Engine& other) = delete;

		Engine& operator=(Engine&& other) = delete;

		friend LRESULT CALLBACK ::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// ACCESSOR
		time::TimeManager * const				GetTimeManager()			{ return time_manager_.get(); };
		renderer::DirectXRenderer * const		GetRenderer()				{ return direct_x_renderer_.get(); };
		input::InputManager * const				GetInputManager()			{ return input_manager_.get(); }
		windows::WindowsMessageManager * const	GetWindowsMessageManager()	{ return windows_message_manager_.get(); };
		command::CommandManager * const			GetCommandManager()			{ return command_manager_.get(); }
		logging::LogManager * const				GetLogManager()				{ return log_manager_.get(); }

		HINSTANCE GetInstanceHandle() const { return _instance_handle; }

		// TODO: window manager

		HWND GetMainWindowHandle() const { return _main_window_handle; }

		bool IsWindowMaximized() const { return _is_window_maximized; };

		bool IsWindowMinimized() const { return _is_window_minimized; };

		bool IsWindowResizing() const { return _is_window_resizing; };

		bool IsWindowFullscreen() const { return _is_window_fullscreen; };

		bool IsDestroyed() const { return _instance == nullptr; };

		// MUTATOR

		Status Run();

		virtual bool Initialize(HINSTANCE hInstance);

		// Called to begin orderly shutdown.
		void Shutdown();

		void Destroy();

		// TODO: make a window manager;

		void Resize(int width, int height);

		void SetIsWindowResizing(bool is_resizing) { _is_window_resizing = is_resizing; }

		void SetIsWindowMaximized(bool is_maximized) { _is_window_maximized = is_maximized; }

		void SetIsWindowMinimized(bool is_minimized) { _is_window_minimized = is_minimized; }
	};
}

