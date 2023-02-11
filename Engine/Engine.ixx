// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

#undef min

export module Engine;

export import <array>;
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

export import gsl;
export import InputModel;
export import Task;

import MakeUnique;

using namespace std::literals;

using mt::input::InputManagerInterface;
using mt::renderer::RendererInterface;
using mt::windows::WindowsMessageManagerInterface;
using mt::windows::WindowManagerInterface;
using mt::time::TimeManagerInterface;

using std::unique_ptr;

using namespace mt::error;
using namespace mt::memory;
using namespace mt::task;

export namespace mt
{
	class Engine
	{
		std::unique_ptr<Error> _error = make_unique_nothrow<Error>();

		unique_ptr<InputManagerInterface>	_input_manager 	= nullptr;
		unique_ptr<TimeManagerInterface>	_time_manager 	= nullptr;
		unique_ptr<WindowManagerInterface>	_window_manager = nullptr;
		unique_ptr<RendererInterface>		_renderer		= nullptr;
		unique_ptr<Game>					_game 			= nullptr;

		// Shutdown is checked to see if Tick should keep ticking, on true ticking stops and Tick() returns
		std::atomic<bool> _is_shutting_down = false;

	protected:	
		static Engine* _instance;

	public:
		Engine() noexcept;
		~Engine() noexcept;
		Engine(const Engine& other) noexcept = default;
		Engine(Engine&& other) noexcept = default;
		Engine& operator=(const Engine& other) noexcept = default;
		Engine& operator=(Engine&& other) noexcept = default;
		 
		// ACCESSOR
		[[nodiscard]] InputManagerInterface * 	getInputManager() 	noexcept	{ return _input_manager.get(); }
		[[nodiscard]] RendererInterface * 		getRenderer() 		noexcept	{ return _renderer.get(); };
		[[nodiscard]] WindowManagerInterface * 	getWindowManager() 	noexcept	{ return _window_manager.get(); };
		[[nodiscard]] TimeManagerInterface * 	getTimeManager() 	noexcept	{ return _time_manager.get(); };
		[[nodiscard]] Game * 					getGame() 			noexcept	{ return _game.get(); };

		[[nodiscard]] static bool isDestroyed() noexcept { return _instance == nullptr; };
		[[nodiscard]] bool isShuttingDown() const noexcept { return _is_shutting_down.load(); }
		// MUTATOR

		[[nodiscard]] std::expected<void, std::unique_ptr<Error>> run(std::unique_ptr<Game> game) noexcept;

		// Called to begin orderly shutdown.
		void shutdown() noexcept;

		void crash(mt::error::Error error) noexcept;
	};




}
