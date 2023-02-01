// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

#undef min

export module Engine;

export import <memory>;
export import <chrono>;

export import Constants;
export import Error;
export import Game;

export import RendererInterface;
export import TimeManagerInterface;
export import InputManagerInterface;
export import WindowManagerInterface;
export import WindowsMessageManagerInterface;

export import StopWatch;

export import Task;

export using namespace std::literals::chrono_literals;

using mt::input::InputManagerInterface;
using mt::renderer::RendererInterface;
using mt::windows::WindowsMessageManagerInterface;
using mt::windows::WindowManagerInterface;
using mt::time::TimeManagerInterface;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

export namespace mt
{
	class Engine
	{
		friend LRESULT CALLBACK::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		const std::unique_ptr<mt::windows::WindowManagerInterface>			_window_manager;
		const std::unique_ptr<windows::WindowsMessageManagerInterface>	_windows_message_manager;
		const std::unique_ptr<input::InputManagerInterface>				_input_manager;
		const std::unique_ptr<time::TimeManagerInterface>				_time_manager;
		const std::unique_ptr<renderer::RendererInterface>				_renderer;

		std::chrono::steady_clock::duration _time_since_stat_update = 0ns;

		// Shutdown is checked to see if Tick should keep ticking, on true ticking stops and Tick() returns
		std::atomic<bool> _is_shutting_down = false;

	protected:	
		static Engine* _instance;

		std::thread _engine_tick_thread;

		[[nodiscard]] std::expected<void, mt::error::Error> _tick(
			mt::time::StopWatch* tick_time,
			mt::time::StopWatch* update_time, 
			mt::time::StopWatch* render_time, 
			mt::time::StopWatch* frame_time,
			mt::time::StopWatch* input_time,
			mt::Game& game
		) noexcept;


	public:
		// Big 5
		Engine(HINSTANCE hInstance);
		~Engine() noexcept;
		Engine(const Engine& other) noexcept = delete;
		Engine(Engine&& other) noexcept = delete;
		Engine& operator=(const Engine& other) noexcept = delete;
		Engine& operator=(Engine&& other) noexcept = delete;
		 
		// ACCESSOR
		InputManagerInterface * const	getInputManager() noexcept	{ return _input_manager.get(); }
		RendererInterface * const		getRenderer() noexcept		{ return _renderer.get(); };
		WindowManagerInterface * const	getWindowManager() noexcept	{ return _window_manager.get(); };
		TimeManagerInterface * const	getTimeManager() noexcept	{ return _time_manager.get(); };

		WindowsMessageManagerInterface * const	getWindowsMessageManager() noexcept	{
			return _windows_message_manager.get();
		};

		bool isDestroyed() const noexcept { return _instance == nullptr; };
		bool isShuttingDown() const noexcept { return _is_shutting_down.load(); }
		// MUTATOR

		[[nodiscard]] std::expected<void, mt::error::Error> run(Game& game) noexcept;

		// Called to begin orderly shutdown.
		void shutdown() noexcept;

	};
}
