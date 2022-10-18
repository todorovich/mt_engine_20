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
	namespace input { class InputManager; };
	namespace command { class CommandManager; };
	namespace logging { class LogManager; };
	namespace renderer { class DirectXRenderer; };
	namespace time { class TimeManager; };
	namespace windows { 
		class WindowManager;
		class WindowsMessageManager; 
	};

	class Engine
	{
		friend LRESULT CALLBACK::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		const std::unique_ptr<command::CommandManager>			command_manager_;
		const std::unique_ptr<logging::LogManager>				log_manager_;
		const std::unique_ptr<input::InputManager>				input_manager_;
		const std::unique_ptr<time::TimeManager>				time_manager_;
		const std::unique_ptr<renderer::DirectXRenderer>		direct_x_renderer_;
		const std::unique_ptr<windows::WindowManager>			window_manager_;
		const std::unique_ptr<windows::WindowsMessageManager>	windows_message_manager_;

		time::Duration _time_since_stat_update = time::Duration(0);
		
		volatile bool _is_shutting_down = false;    // Shutdown is checked to see if Tick should keep ticking, on true ticking stops and Tick() returns

	protected:	
		static Engine* _instance;

		std::thread _engine_tick_thread;

		// CALLBACKS
		virtual void _Update() {};
		virtual void _Draw() {};

		void _Tick();
		void _UpdateFrameStatisticsNoTimeCheck(bool was_rendered);
	public:
		
		// Big 5
		Engine();

		virtual ~Engine();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;
		 
		// ACCESSOR
		command::CommandManager * const			GetCommandManager()			{ return command_manager_.get(); }
		input::InputManager * const				GetInputManager()			{ return input_manager_.get(); }
		logging::LogManager * const				GetLogManager()				{ return log_manager_.get(); }
		renderer::DirectXRenderer * const		GetRenderer()				{ return direct_x_renderer_.get(); };
		windows::WindowManager * const			GetWindowManager()			{ return window_manager_.get(); };
		windows::WindowsMessageManager * const	GetWindowsMessageManager()	{ return windows_message_manager_.get(); };
		time::TimeManager * const				GetTimeManager()			{ return time_manager_.get(); };

		bool IsDestroyed() const { return _instance == nullptr; };

		// MUTATOR

		virtual bool Initialize(HINSTANCE hInstance);

		Status Run();

		// Called to begin orderly shutdown.
		void Shutdown();

		void Destroy();
	};
}

