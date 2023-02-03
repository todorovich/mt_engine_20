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
export import gsl;

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

		std::unique_ptr<mt::windows::WindowManagerInterface>		_window_manager;
		std::unique_ptr<windows::WindowsMessageManagerInterface>	_windows_message_manager;
		std::unique_ptr<input::InputManagerInterface>				_input_manager;
		std::unique_ptr<time::TimeManagerInterface>					_time_manager;
		std::unique_ptr<renderer::RendererInterface>				_renderer;

		std::chrono::steady_clock::duration _time_since_stat_update = 0ns;

		// Shutdown is checked to see if Tick should keep ticking, on true ticking stops and Tick() returns
		std::atomic<bool> _is_shutting_down = false;

	protected:	
		static Engine* _instance;

		std::thread _engine_tick_thread;

		[[nodiscard]] std::expected<void, mt::error::Error> _tick(
			gsl::not_null<mt::time::StopWatch*> tick_time,
			gsl::not_null<mt::time::StopWatch*> update_time,
			gsl::not_null<mt::time::StopWatch*> render_time,
			gsl::not_null<mt::time::StopWatch*> frame_time,
			gsl::not_null<mt::time::StopWatch*> input_time,
			mt::Game& game
		) noexcept;


	public:
		// Big 5
		Engine(HINSTANCE hInstance = nullptr);
		~Engine() noexcept;
		Engine(const Engine& other) noexcept = default;
		Engine(Engine&& other) noexcept = default;
		Engine& operator=(const Engine& other) noexcept = default;
		Engine& operator=(Engine&& other) noexcept = default;
		 
		// ACCESSOR
		InputManagerInterface * 	getInputManager() noexcept	{ return _input_manager.get(); }
		RendererInterface * 		getRenderer() noexcept		{ return _renderer.get(); };
		WindowManagerInterface * 	getWindowManager() noexcept	{ return _window_manager.get(); };
		TimeManagerInterface * 	getTimeManager() noexcept	{ return _time_manager.get(); };

		WindowsMessageManagerInterface * 	getWindowsMessageManager() noexcept	{
			return _windows_message_manager.get();
		};

		static [[nodiscard]] bool isDestroyed() noexcept { return _instance == nullptr; };
		[[nodiscard]] bool isShuttingDown() const noexcept { return _is_shutting_down.load(); }
		// MUTATOR

		[[nodiscard]] std::expected<void, mt::error::Error> run(Game& game) noexcept;

		// Called to begin orderly shutdown.
		void shutdown() noexcept;

		void crash(mt::error::Error error) noexcept;
	};
}
