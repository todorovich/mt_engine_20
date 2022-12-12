module;

#include <windows.h>

// Fuck macros. This wasted like 30 minutes of my time.
#undef RELATIVE

export module EngineDemo;

import Engine;
import DirectXUtility;

export namespace mt
{
	export class EngineDemo : public Engine
	{
	public:
		EngineDemo() = default;
		~EngineDemo() = default;

		virtual bool initialize(HINSTANCE hInstance) override;

	private:
		virtual void _update() override;
		virtual void _draw() override;

		void map_input_controls();
	};

	bool EngineDemo::initialize(HINSTANCE hInstance)
	{
		if (!Engine::initialize(hInstance))
			return false;

		map_input_controls();

		return true;
	}

	void EngineDemo::_update() {}

	void EngineDemo::_draw() {}

}

int WINAPI WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE prevInstance, [[maybe_unused]] PSTR cmdLine, [[maybe_unused]] int showCmd)
{
	try
	{
		mt::EngineDemo engine_demo = mt::EngineDemo();

		engine_demo.initialize(hInstance);

		engine_demo.run();

		engine_demo.destroy();

		return 0;
	}
	catch (mt::renderer::DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}