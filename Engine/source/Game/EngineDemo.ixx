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

		virtual bool _Initialize(HINSTANCE hInstance) override;

	private:
		virtual void _OnResize() override;
		virtual void _Update() override;
		virtual void _Draw() override;

	};

	bool EngineDemo::_Initialize(HINSTANCE hInstance)
	{
		if (!Engine::_Initialize(hInstance))
			return false;

		return true;
	}

	void EngineDemo::_OnResize()
	{}

	void EngineDemo::_Update()
	{}

	void EngineDemo::_Draw()
	{}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		mt::EngineDemo::Initialize(hInstance);

		mt::EngineDemo::Run();

		mt::EngineDemo::Destroy();

		return 0;
	}
	catch (mt::renderer::DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}