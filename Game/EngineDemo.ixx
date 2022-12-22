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
	export class EngineDemo : public Game
	{
		Engine& _engine;

	public:
		virtual void physicsUpdate() override {};
		virtual void inputUpdate() override {};
		virtual void renderUpdate() override {};

		void map_input_controls();

		EngineDemo(Engine& engine)
			: _engine(engine)
		{
			map_input_controls();
		}

		virtual ~EngineDemo() {}
	};
}

int WINAPI WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE prevInstance, [[maybe_unused]] PSTR cmdLine, [[maybe_unused]] int showCmd)
{
	try
	{
		mt::Engine engine = mt::Engine(hInstance);

		mt::EngineDemo engine_demo = mt::EngineDemo(engine);

		return static_cast<int>(engine.run(engine_demo));
	}
	catch (mt::renderer::DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}