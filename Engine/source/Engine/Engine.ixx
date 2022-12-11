// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

export module Engine;

export import Time;
export import Status;
export import std.core;

export import Debug;

export using namespace std::literals::chrono_literals;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

export namespace mt
{
	namespace input { class InputManager; };
	namespace renderer { class DirectXRenderer; };
	namespace time { class TimeManager; class StopWatch; };
	namespace windows { 
		class WindowManager;
		class WindowsMessageManager; 
	};
	
	class Engine
	{
		friend LRESULT CALLBACK::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		const std::unique_ptr<input::InputManager>				_input_manager;
		const std::unique_ptr<renderer::DirectXRenderer>		_direct_x_renderer;
		const std::unique_ptr<windows::WindowManager>			_window_manager;
		const std::unique_ptr<windows::WindowsMessageManager>	_windows_message_manager;
		const std::unique_ptr<time::TimeManager>				_time_manager;

		time::Duration _time_since_stat_update = time::Duration(0);
		
		volatile bool _is_shutting_down = false;    // Shutdown is checked to see if Tick should keep ticking, on true ticking stops and Tick() returns

	protected:	
		static Engine* _instance;

		std::thread _engine_tick_thread;

		// CALLBACKS
		virtual void _update() {};
		virtual void _draw() {};

		void _tick(
			mt::time::StopWatch* tick_time,
			mt::time::StopWatch* update_time, 
			mt::time::StopWatch* render_time, 
			mt::time::StopWatch* frame_time
		);
		void _updateFrameStatisticsNoTimeCheck(bool was_rendered);
	public:
		
		// Big 5
		Engine();

		virtual ~Engine();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;
		 
		// ACCESSOR
		input::InputManager * const				getInputManager()			{ return _input_manager.get(); }
		renderer::DirectXRenderer * const		getRenderer()				{ return _direct_x_renderer.get(); };
		windows::WindowManager * const			getWindowManager()			{ return _window_manager.get(); };
		windows::WindowsMessageManager * const	getWindowsMessageManager()	{ return _windows_message_manager.get(); };
		time::TimeManager * const				getTimeManager()			{ return _time_manager.get(); };

		bool isDestroyed() const { return _instance == nullptr; };

		// MUTATOR

		virtual bool initialize(HINSTANCE hInstance);

		Status run();

		// Called to begin orderly shutdown.
		void shutdown();

		void destroy();
	};
}

