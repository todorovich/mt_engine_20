// Copyright 2024 Micho Todorovich, all rights reserved.
export module Engine;

export import std;

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
//import Windows;

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
		static Engine* _instance;
		static std::atomic<bool> _is_shutting_down;
		static std::atomic<bool> _should_shut_down;

		std::error_condition _error = std::error_condition(
			static_cast<int>(ErrorCode::ERROR_UNINITIALIZED),
			engineErrorCategory()
		);

		std::atomic<std::shared_ptr<InputManagerInterface>>		_input_manager 	= nullptr;
		std::atomic<std::shared_ptr<TimeManagerInterface>>		_time_manager 	= nullptr;
		std::atomic<std::shared_ptr<WindowManagerInterface>>	_window_manager = nullptr;
		std::atomic<std::shared_ptr<RendererInterface>>			_renderer		= nullptr;
		std::atomic<std::shared_ptr<Game>>						_game 			= nullptr;


	protected:
		Engine(const Engine& other) = default;
		Engine& operator=(const Engine& other) = default;
		Engine(Engine&& other) = default;
		Engine& operator=(Engine&& other) = default;

		void setInputManager(std::shared_ptr<InputManagerInterface> input_manager) noexcept {
			_input_manager.store(input_manager, std::memory_order_release);
		}

		void setTimeManager(std::shared_ptr<TimeManagerInterface> time_manager) noexcept {
			_time_manager.store(time_manager, std::memory_order_release);
		}

		void setWindowManager(std::shared_ptr<WindowManagerInterface> window_manager) noexcept {
			_window_manager.store(window_manager, std::memory_order_release);
		}

		void setRenderer(std::shared_ptr<RendererInterface> renderer) noexcept {
			_renderer.store(renderer, std::memory_order_release);
		}

		void setGame(std::shared_ptr<Game> game) noexcept {
			_game.store(game, std::memory_order_release);
		}

		Engine() noexcept = default;

	public:
		friend TimeManagerInterface;

		~Engine() noexcept;

		// ACCESSOR
		[[nodiscard]] InputManagerInterface * getInputManager() const noexcept  {
			return _input_manager.load(std::memory_order_acquire).get();
		}

		[[nodiscard]] RendererInterface * getRenderer() const noexcept {
			return _renderer.load(std::memory_order_acquire).get();
		}

		[[nodiscard]] WindowManagerInterface * getWindowManager() const noexcept {
			return _window_manager.load(std::memory_order_acquire).get();
		}

		[[nodiscard]] TimeManagerInterface * getTimeManager() const noexcept {
			return _time_manager.load(std::memory_order_acquire).get();
		}

		[[nodiscard]] Game * getGame() noexcept {
			return _game.load(std::memory_order_acquire).get();
		}

		[[nodiscard]] static bool isDestroyed() noexcept { return _instance == nullptr; }
		[[nodiscard]] bool isShuttingDown() const noexcept { return _is_shutting_down.load(); }
		[[nodiscard]] bool shouldShutDown() const noexcept { return _should_shut_down.load(); }
		[[nodiscard]] std::error_condition getError() const noexcept { return _error; }

		// MUTATOR
		[[nodiscard]] std::expected<void, std::error_condition> run(std::shared_ptr<Game> game) noexcept;
		// Called to begin orderly shutdown.
		void shutdown() noexcept { _should_shut_down = true; }
		
		// Called to begin orderly shutdown.
		void setIsShuttingDown() { _is_shutting_down = true; }
		std::error_condition crash(std::error_condition error) noexcept;
	};
}
