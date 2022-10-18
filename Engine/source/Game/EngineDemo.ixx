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

		virtual bool Initialize(HINSTANCE hInstance) override;

	private:
		virtual void _Update() override;
		virtual void _Draw() override;

	};

	bool EngineDemo::Initialize(HINSTANCE hInstance)
	{
		if (!Engine::Initialize(hInstance))
			return false;

		return true;
	}

	void EngineDemo::_Update() {}

	void EngineDemo::_Draw() {}
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

		engine_demo.Initialize(hInstance);

		engine_demo.Run();

		engine_demo.Destroy();

		return 0;
	}
	catch (mt::renderer::DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}