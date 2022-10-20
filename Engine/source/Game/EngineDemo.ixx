module;

#include <windows.h>
#include <crtdbg.h>

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

	};

	bool EngineDemo::initialize(HINSTANCE hInstance)
	{
		if (!Engine::initialize(hInstance))
			return false;

		return true;
	}

	void EngineDemo::_update() {}

	void EngineDemo::_draw() {}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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