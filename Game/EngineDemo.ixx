// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <windows.h>

// Fuck macros. This wasted like 30 minutes of my time.
#undef RELATIVE

export module EngineDemo;

export import <map>;

export import Game;
export import Engine;
export import DirectXUtility;
export import Task;
export import InputHandlers;

using namespace mt::task;

export namespace mt
{
	struct Keys
	{
		static const std::size_t QUIT;
		static const std::size_t TOGGLE_RELATIVE_MOUSE;
		static const std::size_t WALK_FORWARD;
		static const std::size_t WALK_BACKWARD;
		static const std::size_t STRAFE_LEFT;
		static const std::size_t STRAFE_RIGHT;
		static const std::size_t FLY_UP;
		static const std::size_t FLY_DOWN;
	};

	class EngineDemo : public Game
	{
		Engine& _engine;
		std::map<std::size_t, std::unique_ptr<Task>> _tasks;
		std::unique_ptr<TwoDimensionalInputTask> _mouse_look;

	public:
		virtual void physicsUpdate() noexcept override {};
		virtual void inputUpdate() noexcept override {};
		virtual void renderUpdate() noexcept override {};

		void map_input_controls() noexcept;

		EngineDemo(mt::Engine& engine) noexcept
			: _engine(engine)
		{
			_tasks.insert({mt::Keys::QUIT, std::make_unique<mt::Quit>(_engine)});
			_tasks.insert({mt::Keys::TOGGLE_RELATIVE_MOUSE, std::make_unique<mt::ToggleRelativeMouse>(_engine)});
			_tasks.insert({
				mt::Keys::WALK_FORWARD,
				std::make_unique<mt::WalkForward>(_engine, 1.0f, _engine.getTimeManager()->getRenderInterval())
			});
			_tasks.insert({
				mt::Keys::WALK_BACKWARD,
				std::make_unique<WalkBackward>(_engine, 1.0f, _engine.getTimeManager()->getRenderInterval())
			});
			_tasks.insert({
				mt::Keys::STRAFE_LEFT,
				std::make_unique<StrafeLeft>(_engine, 1.0f, _engine.getTimeManager()->getRenderInterval())
			});
			_tasks.insert({
				mt::Keys::STRAFE_RIGHT,
				std::make_unique<StrafeRight>(_engine, 1.0f, _engine.getTimeManager()->getRenderInterval())
			});
			_tasks.insert({
				mt::Keys::FLY_UP,
				std::make_unique<FlyUp>(_engine, 1.0f, _engine.getTimeManager()->getRenderInterval())
			});
			_tasks.insert({
				mt::Keys::FLY_DOWN,
				std::make_unique<FlyDown>(_engine, 1.0f, _engine.getTimeManager()->getRenderInterval())
			});
			_mouse_look = std::make_unique<MouseLook>(_engine);

			map_input_controls();
		}

		virtual ~EngineDemo() noexcept = default;
	};
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	[[maybe_unused]] HINSTANCE prevInstance,
	[[maybe_unused]] PSTR cmdLine,
	[[maybe_unused]] int showCmd
)
{
	mt::Engine engine = mt::Engine();

	mt::EngineDemo engine_demo = mt::EngineDemo(engine);

	if (auto expected = engine.run(engine_demo); expected)
		return 0;
	else
		// TODO: do something with the errors returned. Also make that a better data structure.
		return static_cast<int>(expected.error()->getErrorCode());
}