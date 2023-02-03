module;

#include "DirectXMath.h"

export module InputHandlers;

export import Engine;
export import Task;

using namespace std::literals;

export namespace mt
{
	class Quit : public mt::task::Task
	{
		mt::Engine& _engine;
	public:
		Quit(mt::Engine& engine)
			: _engine(engine)
		{}

		virtual std::expected<void, mt::error::Error> operator()() override
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

		virtual std::expected<void, mt::error::Error> operator()() override
		{
			_engine.getInputManager()->toggleRelativeMouse();
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

		virtual std::expected<void, mt::error::Error> operator()() override
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

		virtual std::expected<void, mt::error::Error> operator()() override
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

		virtual std::expected<void, mt::error::Error> operator()() override
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

		virtual std::expected<void, mt::error::Error> operator()(int x, int y) override
		{
			auto& camera = _engine.getRenderer()->getCurrentCamera();

			// TODO: This should be fov dependant.
			// Make each pixel correspond to 1/100th of a degree.
			float dx = DirectX::XMConvertToRadians(0.01f * static_cast<float>(x));
			float dy = DirectX::XMConvertToRadians(0.01f * static_cast<float>(y));

			camera.pitch(dy);
			camera.rotateY(dx);
			return {};
		};
	};
}