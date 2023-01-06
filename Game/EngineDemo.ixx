module;

#include <windows.h>

// Fuck macros. This wasted like 30 minutes of my time.
#undef RELATIVE

export module EngineDemo;

import Game;
import Engine;
import DirectXUtility;

export namespace mt
{
	class EngineDemo : public Game
	{
		Engine& _engine;

	public:
		virtual void physicsUpdate() noexcept override {};
		virtual void inputUpdate() noexcept override {};
		virtual void renderUpdate() noexcept override {};

		void map_input_controls() noexcept;

		EngineDemo(Engine& engine) noexcept
			: _engine(engine)
		{
			map_input_controls();
		}

		virtual ~EngineDemo() noexcept {}
	};
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	[[maybe_unused]] HINSTANCE prevInstance,
	[[maybe_unused]] PSTR cmdLine,
	[[maybe_unused]] int showCmd
)
{
	try
	{
		mt::Engine engine = mt::Engine(hInstance);

		mt::EngineDemo engine_demo = mt::EngineDemo(engine);

		if (auto expected = engine.run(engine_demo); expected)
			return 0;
		else
			return static_cast<int>(expected.error().error_code);
	}
	catch (mt::renderer::DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}