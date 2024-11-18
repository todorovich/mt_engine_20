module;

#include "DirectXMath.h"

export module InputHandlers;

import std;

export import Engine;
export import Task;

import InputModel;

using namespace std::literals;
using namespace std::numbers;

export namespace mt
{
	class Quit : public mt::task::Task
	{
		mt::Engine& _engine;
	public:
		Quit(mt::Engine& engine)
			: _engine(engine)
		{}

		virtual std::expected<void, std::error_condition> operator()() override
		{
			_engine.shutdown();
			return {};
		};
	};

	class ToggleRelativeMouse : public mt::task::Task
	{
		mt::Engine& _engine;
	public:
		ToggleRelativeMouse(mt::Engine& engine)
			: _engine(engine)
		{}

		virtual std::expected<void, std::error_condition> operator()() override
		{
			_engine.getInputManager()->acceptInput(
				mt::input::model::InputType(
					mt::input::model::InputDevice::MOUSE,
					mt::input::model::InputDataType::NO_DATA_TYPE,
					mt::input::model::InputContext::RELATIVE
				)
			);
			return {};
		};
	};

	class WalkForward : public mt::task::Task
	{
		mt::Engine& _engine;
		float _adjusted_walk_speed;
	public:
		WalkForward(mt::Engine& engine, float walk_speed, std::chrono::steady_clock::duration delta_time)
			: _engine(engine)
			, _adjusted_walk_speed(walk_speed / (1s / delta_time))
		{}

		virtual std::expected<void, std::error_condition> operator()() override
		{
			_engine.getRenderer()->getCurrentCamera().walk(_adjusted_walk_speed);
			return {};
		};
	};

	class WalkBackward: public WalkForward
	{
	public:
		WalkBackward(mt::Engine& engine, float walk_speed, std::chrono::steady_clock::duration delta_time)
			: WalkForward(engine, -walk_speed, delta_time)
		{}
	};

	class StrafeRight : public mt::task::Task
	{
		mt::Engine& _engine;
		float _adjusted_walk_speed;
	public:
		StrafeRight(mt::Engine& engine, float walk_speed, std::chrono::steady_clock::duration delta_time)
			: _engine(engine)
			, _adjusted_walk_speed(walk_speed / (1s / delta_time))
		{}

		virtual std::expected<void, std::error_condition> operator()() override
		{
			_engine.getRenderer()->getCurrentCamera().strafe(_adjusted_walk_speed);
			return {};
		};
	};

	class StrafeLeft : public StrafeRight
	{
	public:
		StrafeLeft(mt::Engine& engine, float walk_speed, std::chrono::steady_clock::duration delta_time)
			: StrafeRight(engine, -walk_speed, delta_time)
		{}
	};

	class FlyUp : public mt::task::Task
	{
		mt::Engine& _engine;
		float _adjusted_walk_speed;
	public:
		FlyUp(mt::Engine& engine, float fly_speed, std::chrono::steady_clock::duration delta_time)
			: _engine(engine)
			, _adjusted_walk_speed(fly_speed / (1s / delta_time))
		{}

		virtual std::expected<void, std::error_condition> operator()() override
		{
			_engine.getRenderer()->getCurrentCamera().fly(_adjusted_walk_speed);
			return {};
		};
	};

	class FlyDown : public FlyUp
	{
	public:
		FlyDown(mt::Engine& engine, float fly_speed, std::chrono::steady_clock::duration delta_time)
			: FlyUp(engine, -fly_speed, delta_time)
		{}
	};

	class MouseLook : public mt::task::TwoDimensionalInputTask
	{
		mt::Engine& _engine;
	public:
		MouseLook(mt::Engine& engine)
			: _engine(engine)
		{}

		//
		virtual std::expected<void, std::error_condition> operator()(int x, int y) override
		{
			auto width = _engine.getWindowManager()->getWindowWidth();
			auto height = _engine.getWindowManager()->getWindowHeight();

			auto degreesToRadians = (pi_v<float> / 180.0f);

			auto& camera = _engine.getRenderer()->getCurrentCamera();

			auto half_h_fov = 0.5f * camera.getFovX();
			auto x_tangent = tanf(half_h_fov);
			const float x_base = (2.0f * x * x_tangent) / (width / 2.0f);

			auto half_v_fov = 0.5f * camera.getFovY();
			auto y_tangent = tanf(half_v_fov);
			const float y_base = (2.0f * y * y_tangent) / (height / 2.0f);

			float dx = atanf(x_base);
			float dy = atanf(y_base);

			camera.pitch(dy);
			camera.rotateY(dx);

			return {};
		};
	};
}